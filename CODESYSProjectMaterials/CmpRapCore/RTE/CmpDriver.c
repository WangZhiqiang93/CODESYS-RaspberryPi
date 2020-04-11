/*
Copyright by 3S Smart Software Solutions GmbH, 87439 Kempten, Germany
Author: A. Fauter
*/

/*
This file is ready to use, but does nothing. 
*/

#include "CmpStd.h"
#include "CmpItf.h"

#include "CmpRapCoreDep.h"

#include "CmpDriver.h"
#include "CmpDriverItf.h"


#define MAXIMUM_STRING_BUFFER 80

//globals to export
void* g_pDefaultDispatch = NULL;

DLL_DECL int CDECL ComponentEntry(INIT_STRUCT *pInitStruct);

ExportedFct s_ExpFctTable[] = 
{
	{ComponentEntry,"ComponentEntry"},
	{NULL,""}
};

//forward decls
NTSTATUS ntDeviceControl(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp);
NTSTATUS ntCreateFile(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp);
VOID DriverUnload(IN PDRIVER_OBJECT pDriverObject);
NTSTATUS ntShutdown(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp);

// This is the definition for the routine, that will be called by the RT-PLC as a callback.
unsigned long ntCallback(CallbackInfo* pcbi, void* pPar)
{
	if(pcbi->ulSecurityId1 != 0x436F4465 || pcbi->ulSecurityId2 != 0x53797320)
	 	return STATUS_SUCCESS; /*This is not a CoDeSys-related programm, that calls the function.*/

	switch(pcbi->ulCommand)
	{
        case DRV_CMD_VERSIONCHECK:
        {
			KernelModuleDesc* pKMDesc = (KernelModuleDesc*)pcbi->ulCmdParam;
			unsigned short us = (unsigned short)CMP_VERSION;
			int i = 0;
            *(unsigned long*)pcbi->pAdditionalCmdData = (us << 16) | 2000; //pAdditionalCmdData points to a dword where the driver inserts its version.
                                                        //The HIWORD is the number of functionpointers, the driver will insert.

			/*Enter all my fuctions to the passed description. Normally a Cmp needs to enter only one function:
				ComponentEntry.*/
			while(s_ExpFctTable[i].pfFct != NULL && i < MAX_EXPORTED_FUNCTIONS)
			{
				pKMDesc->aefFctTable[i].pfFct = s_ExpFctTable[i].pfFct;
				strncpy(pKMDesc->aefFctTable[i].szName,s_ExpFctTable[i].szName,sizeof(pKMDesc->aefFctTable[i].szName)-1);
				i++;
			}

            break;    
        }
		case DRV_CMD_INVALID_CMD:
		default:
			return STATUS_SUCCESS; //what's that for a call?
	}

	return 1;
}

int ScanPciBusForSpecificDevice(unsigned long ulVendorId, unsigned long ulDeviceId, unsigned long ulIdx, PCI_COMMON_CONFIG* pPciCfg)
{
	int i, iBus = 0, iDevice = 0, iFunction = 0, iValid = 1;
	unsigned long ulState = 0;
	int iNumTotal = 0;
	PCI_COMMON_CONFIG pcicc; 
	PCI_SLOT_NUMBER pcisSlotNr;
	int iCurIdx = 0;

	for( iBus = 0; iBus >= 0 && iValid != 0; iBus++)
	{
		for(iDevice = 0; iValid != 0 && iDevice < PCI_MAX_DEVICES; iDevice++)
		{
			for(iFunction = 0; iFunction < PCI_MAX_FUNCTION; iFunction++)
			{
                pcisSlotNr.u.bits.Reserved = 0;
                pcisSlotNr.u.bits.DeviceNumber = iDevice;
                pcisSlotNr.u.bits.FunctionNumber = iFunction;

                pcicc.VendorID = PCI_INVALID_VENDORID;

                ulState = HalGetBusData(PCIConfiguration,iBus,pcisSlotNr.u.AsULONG,&pcicc,sizeof(pcicc));

                if(ulState == 0)
                {
                    iValid = 0; //Busnumber is invalid
                    break;
                }
                if((ulState == 2) || (PCI_INVALID_VENDORID == pcicc.VendorID))
                    continue; //No device found at this place.

				if(pcicc.VendorID == (unsigned short)ulVendorId && pcicc.DeviceID == (unsigned short)ulDeviceId)
				{
					memcpy(pPciCfg,&pcicc,sizeof(pcicc));
					iValid = 0;
					iNumTotal++;
					break;
				}
            }
        }
    }
    return iNumTotal;
}


NTSTATUS ntCreateDevice(	IN	PDRIVER_OBJECT		pDriverObject,
							OUT	PDEVICE_OBJECT*		ppDeviceObject,
							OUT	void**	ppsExtension )
// PURPOSE
//	creates the device object for the driver
// ADDITIONAL INFORMATION
//	As I don't know what NTSTATUS values "IoCreateSymbolicLink" can return, I also don't know what
//	NTSTATUS values can be returned by this function.
// PARAMETERS
//	pDriverObject:	the driver's object
//	ppDeviceObject:	pointer to driver's device object - returned by this function, if successfull
//	ppsExtension:	the driver's device extension     - returned by this function, if successfull
// RETURN VALUE
//	STATUS_SUCCESS:	device object was successfully created
//	If the routine wasn't successfull then another NTSTATUS value is returned. Probably one of these:
//		STATUS_INSUFFICIENT_RESOURCES
//		STATUS_OBJECT_NAME_EXISTS
//		STATUS_OBJECT_NAME_COLLISION
{

    WCHAR          deviceNameBuffer[] = L"\\Device\\"MY_CMPDRV_NAME_L; //TODO: change to own drivername
                                                            //TODO: change #define MY_IODRV_NAME XXXXXX in IOdrvInterface.h also! 
    UNICODE_STRING deviceNameUnicodeString;
    WCHAR          deviceLinkBuffer[] = L"\\DosDevices\\"MY_CMPDRV_NAME_L; //TODO: change to own drivername
    UNICODE_STRING deviceLinkUnicodeString;
	NTSTATUS		ntStatus;		// return value / error code
	BOOLEAN	bHalRes = FALSE;
	unsigned long zero = 0;
	unsigned long ulBaseAddress = 0x0;
	unsigned long ulLength = 0;
	NTSTATUS ntsQueryRegRes = -1;
	
	// create device name and win32 device name strings
	RtlInitUnicodeString(&deviceNameUnicodeString, deviceNameBuffer);
	RtlInitUnicodeString(&deviceLinkUnicodeString, deviceLinkBuffer);

	// Create the device object:
	ntStatus = IoCreateDevice(
		pDriverObject,				// DriverObject created by IO Manager when driver was loaded
		20,	// DeviceExtensionSize  //TODO: change to your own device-extension-size
		&deviceNameUnicodeString,	// DeviceName
		FILE_DEVICE_RTIODRIVERthis,		// DeviceType
		0,							// DeviceCharacteristics
		FALSE,						// Exclusive
		ppDeviceObject );			// DeviceObject returned by the function

	if (!NT_SUCCESS(ntStatus))
	{
		// Creating device object failed.
		KdPrint(("IoCreateDevice failed."));
		return ntStatus;
	}


	// Create a link from our device name to a name in the Win32 namespace.
    ntStatus = IoCreateSymbolicLink (&deviceLinkUnicodeString,
                                         &deviceNameUnicodeString);
	if (!NT_SUCCESS(ntStatus))
	{
		KdPrint(("IoCreateSymLink failed."));
		// Creating a symbolic link failed.
		IoDeleteDevice(*ppDeviceObject);
		*ppDeviceObject = NULL;
		return ntStatus;
	}

	// Fill in the device extension.
	*ppsExtension = (void*)((*ppDeviceObject)->DeviceExtension);

	return ntStatus;
} // ntCreateDevice

PCI_COMMON_CONFIG g_pcicc;
#pragma pack(1)
typedef struct _MSI_CAP_ENTRY
{
	PCI_CAPABILITIES_HEADER Header;
	union
	{
		struct _32BitMessage
		{
			unsigned short MessageControl;
			unsigned long ulMessageAddress;
			unsigned short usMessageData;
		}MessageNoMask_32Bit;
		struct _64BitMessage
		{
			unsigned short MessageControl;
			unsigned __int64 ulMessageAddress;
			unsigned short usMessageData;
		}MessageNoMask_64Bit;
		struct _32BitMessage_Mask
		{
			unsigned short MessageControl;
			unsigned long ulMessageAddress;
			unsigned short usMessageData;
			unsigned short usReserved;
			unsigned long ulMaskBits;
			unsigned long ulPendingBits;
		}MessageMask_32Bit;
		struct _64BitMessage_Mask
		{
			unsigned short MessageControl;
			unsigned __int64 ulMessageAddress;
			unsigned short usMessageData;
			unsigned short usReserved;
			unsigned long ulMaskBits;
			unsigned long ulPendingBits;
		}MessageMask_64Bit;
	}u;
}MSI_CAP_ENTRY;

typedef struct _MSIX_CAP_ENTRY
{
	PCI_CAPABILITIES_HEADER Header;
	unsigned short MessageControl;
	unsigned long ulTableOffsetBIR; // MSIX Table structure offset.
	unsigned long ulPendingOffsetBIR; //PBA Pending Bit Array. BIR: BAR indicator register.
}MSIX_CAP_ENTRY;

#pragma pack()
int g_bMsiEnabled;
int g_bMsi64Bit;
int g_bMsiMask;
unsigned long g_ulMsixTableOffset;
unsigned long g_ulMsixPBAOffset;
int g_iBarIdxTable;
int g_iBarIdxPBA;
int g_bMsixEnable;
unsigned short g_usMsixMesssageControl;
unsigned long* g_pulMsixTable;
unsigned __int64* g_pulPBA;
PVOID KernelGetModuleBase(PCHAR  pModuleName);
PVOID KernelGetProcAddress(char* ModuleBase, PCHAR pFunctionName);
NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING ustrRegistryPath)
// PURPOSE
//	This routine is called, when the driver is loaded by the system.
// PARAMETERS
//	pDriverObject:		Pointer to the driver's object, that was created by the system.
//	ustrRegistryPath:	Path to the driver's registry entry.
// RETURN VALUE
//	STATUS_SUCCESS:	Everything upon driver initialization went fine and the driver is ready for IO.
{

	NTSTATUS			ntStatus;				// return value, also gets return values of calls to other functions
    PDEVICE_OBJECT		pDeviceObject	= NULL;	// pointer to device object
	void*	psExtension		= NULL;	// pointer to device extension
	int iPciDeviceFound = 0;
	PVOID pKernelModule = NULL;
	PVOID pProc = NULL;
	char szModuleName[32];
	char szProcName[32];
	char* pModuleName = &szModuleName[0];
	char* pProcName = &szProcName[0];

	PHYSICAL_ADDRESS addr_max, addr_low,boundary;
	void *ret = NULL;

	memset(szModuleName,0,sizeof(szModuleName));
	memset(szProcName,0,sizeof(szProcName));

	KdPrint((MY_CMPDRV_NAME" :Entering DriverEntry.\r\n"));

	// initialize dispatch routines
	g_pDefaultDispatch = (void*)pDriverObject->MajorFunction[IRP_MJ_INTERNAL_DEVICE_CONTROL];
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ntDeviceControl;
	pDriverObject->MajorFunction[IRP_MJ_CREATE        ] = ntCreateFile;
	pDriverObject->MajorFunction[IRP_MJ_SHUTDOWN      ] = ntShutdown;
#pragma warning(disable : 4028)
	pDriverObject->MajorFunction[IRP_MJ_INTERNAL_DEVICE_CONTROL] = ntCallback;
#pragma warning(default : 4028)
	pDriverObject->DriverUnload = DriverUnload;

	// create device
	ntStatus = ntCreateDevice(pDriverObject, &pDeviceObject, &psExtension);
	if (!NT_SUCCESS(ntStatus))
	{
		KdPrint(("Creating a symbolic link failed"));
		// Creating a symbolic link failed.
		return ntStatus;
	}

	addr_max.HighPart = addr_low.HighPart = addr_low.LowPart = 0;
	addr_max.LowPart = 0xfffff;	/* in first segment for simplicity of startup code */	
	/* boundary.QuadPart = 4096; // not necessary, allocated memory is always page aligned */
	boundary.QuadPart = 0;

	ret = MmAllocateContiguousMemory(512, addr_max);
	//ret = MmAllocateContiguousMemorySpecifyCache(4096, addr_low, addr_max, boundary, MmNonCached);

	if(ret != NULL)
		KdPrint(("MmAllocateContiguousMemorySpecifyCache succeeded: %p\r\n",ret));
	else
		KdPrint(("MmAllocateContiguousMemorySpecifyCache failed.\r\n"));

#if 0
	strcpy(szModuleName,"ntoskrnl.exe");
	strcpy(szModuleName,"ntkrnlmp.exe");
	strcpy(szModuleName,"nt");
	strcpy(szProcName,"Watchdog");

	pKernelModule = KernelGetModuleBase(szModuleName);
	pProc = KernelGetProcAddress(pKernelModule,szProcName);
#endif
	//iPciDeviceFound = ScanPciBusForSpecificDevice(0x8086,0x1533,0,&g_pcicc);
#if 0
	if(iPciDeviceFound)
	{
		if(g_pcicc.Status & 0x10)
		{
			//Capabilities list present. Offset of start of Cap-List in g_pcicc.
			unsigned char ucOffsetCapList = g_pcicc.u.type0.CapabilitiesPtr;
			PCI_CAPABILITIES_HEADER* pcapheader = (PCI_CAPABILITIES_HEADER*)&((char*)&g_pcicc)[ucOffsetCapList];
			while(pcapheader->Next != 0)
			{
				if(pcapheader->CapabilityID == PCI_CAPABILITY_ID_MSIX)
				{
					MSIX_CAP_ENTRY* pmsixcapentry = (MSIX_CAP_ENTRY*)pcapheader;
					PHYSICAL_ADDRESS pa;
					if(pmsixcapentry->MessageControl & 0x8000)
					{ //Enabled.
						g_bMsixEnable = 1;
						g_usMsixMesssageControl = pmsixcapentry->MessageControl;
						g_ulMsixTableOffset = pmsixcapentry->ulTableOffsetBIR & 0xfffffff8;
						g_ulMsixPBAOffset = pmsixcapentry->ulPendingOffsetBIR & 0xfffffff8;
						g_iBarIdxPBA = pmsixcapentry->ulPendingOffsetBIR & 0x7;
						g_iBarIdxTable = pmsixcapentry->ulTableOffsetBIR & 0x7;

						pa.QuadPart = g_pcicc.u.type0.BaseAddresses[g_iBarIdxTable] + g_ulMsixTableOffset;
						g_pulMsixTable = (unsigned long*)MmMapIoSpace(pa,0x1000,MmNonCached);
						pa.QuadPart = g_pcicc.u.type0.BaseAddresses[g_iBarIdxPBA] + g_ulMsixPBAOffset;
						g_pulPBA = (unsigned __int64*)MmMapIoSpace(pa,0x1000,MmNonCached);
					}
					else
					{
						pcapheader = (PCI_CAPABILITIES_HEADER*)&((char*)&g_pcicc)[pcapheader->Next];
						continue; //Disabled.
					}
				}
				if(pcapheader->CapabilityID == PCI_CAPABILITY_ID_MSI)
				{
					MSI_CAP_ENTRY* pmsicapentry = (MSI_CAP_ENTRY*)pcapheader;
					if((pmsicapentry->u.MessageMask_32Bit.MessageControl & 0x0001) == 0)
					{
						pcapheader = (PCI_CAPABILITIES_HEADER*)&((char*)&g_pcicc)[pcapheader->Next];
						continue; //Disabled.
					}
					g_bMsiEnabled = 1;
					if(pmsicapentry->u.MessageMask_32Bit.MessageControl & 0x0100)
					{ //Mask available.
						if(pmsicapentry->u.MessageMask_32Bit.MessageControl & 0x0080)
						{ //64Bit.
							g_bMsi64Bit = 1;
							g_bMsiMask = 1;
						}
						else
						{ //32Bit.
							g_bMsi64Bit = 0;
							g_bMsiMask = 1;
						}
					}
					else
					{ //NoMask.
						if(pmsicapentry->u.MessageMask_32Bit.MessageControl & 0x0080)
						{ //64Bit.
							g_bMsi64Bit = 1;
							g_bMsiMask = 0;
						}
						else
						{ //32Bit.
							g_bMsi64Bit = 0;
							g_bMsiMask = 0;
						}
					}
				}

				pcapheader = (PCI_CAPABILITIES_HEADER*)&((char*)&g_pcicc)[pcapheader->Next];
			}
		}
	}
#endif
	return STATUS_SUCCESS;
} // DriverEntry



NTSTATUS ntCreateFile(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp)
// PURPOSE
//	This routine is called, when a user mode app opens a handle to the driver by calling CreateFile.
//	It does all specific initializations for this opening process.
// PARAMETERS
//	pDriverObject:	pointer to the driver's object
//	pIrp:			pointer to the IRP for the IO call
// RETURN VALUE
//	STATUS_SUCCESS:	at the current state of development this is the only return value
{

	return STATUS_SUCCESS;
} 



VOID DriverUnload(IN PDRIVER_OBJECT pDriverObject)
// PURPOSE
//	This routine is called when the driver is unloaded (e.g. at system shutdown). It frees all
//	resources that the driver has locked.
// PARAMETERS
//	pDriverObject:	pointer to the driver's object, that was created by the system
{

    WCHAR          deviceLinkBuffer[] = L"\\DosDevices\\"MY_CMPDRV_NAME_L; //TODO: change to own driver name.
    UNICODE_STRING deviceLinkUnicodeString;
    BOOLEAN				bConflictDetected;		// needed for IoReportResourceUsage, set to true, if
												// resources have already been claimed by another device

	// We don't need our resources anymore (ports and interrupt).
	IoReportResourceUsage(NULL, pDriverObject, NULL, 0, NULL, NULL, 0, FALSE, &bConflictDetected);

	// Delete the link from our device name to a name in the Win32 namespace.
	RtlInitUnicodeString(&deviceLinkUnicodeString, deviceLinkBuffer);
	IoDeleteSymbolicLink(&deviceLinkUnicodeString);

	// Finally delete our device object
	IoDeleteDevice(pDriverObject->DeviceObject);
} // DriverUnload


NTSTATUS ntDeviceControl(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp)
// PURPOSE
//	This routine is called, if a user mode app makes a call to DeviceIoControl.
// PARAMETERS
//	pDriverObject:	pointer to the driver's object
//	pIrp:			pointer to the IRP for the IO call
// RETURN VALUE
//	Returns a NTSTATUS value.
{

	NTSTATUS			ntStatus	= STATUS_SUCCESS;										// return value / get's return values from function calls
	void*	psExtension	= (void*)pDeviceObject->DeviceExtension;	// pointer to driver's device extension
	PIO_STACK_LOCATION	pIrpStack	= IoGetCurrentIrpStackLocation(pIrp);					// pointer to IRP's stack


	switch (pIrpStack->Parameters.DeviceIoControl.IoControlCode)
	{
		//process a direct command, send by a call to DeviceIOControl from an application.
		case IOCTL_DIRECT_COMMAND:
            switch(((DirectCommand*)(pIrp->AssociatedIrp.SystemBuffer))->Cmd)
            {
                case RTS_CUSTOM_SERVICES:
                {
                    switch(((DirectCommand*)(pIrp->AssociatedIrp.SystemBuffer))->SubCmd)
        			{
                        default:
                            pIrp->IoStatus.Information = 0;
                            break;
                    }
                    break;
                }
		        default:
            	    pIrp->IoStatus.Information = 0;
                    break;
            }
			break;		 
		// unknown IOCTL code
		default:
		{
            ntStatus = STATUS_INVALID_PARAMETER;
			pIrp->IoStatus.Information = 0;
	 	}
	}

	pIrp->IoStatus.Status = ntStatus;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return ntStatus;
} // ntDeviceControl


NTSTATUS ntShutdown(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp)
// PURPOSE
//	Handles shutdown. Is called by the system before going down.
//		but only if a shutdownnotification has been registered.
// PARAMETERS
//	pDriverObject:	pointer to the driver's object
//	pIrp:			pointer to the IRP for the IO call
// RETURN VALUE
//	Returns a NTSTATUS value.
{		    

	
	return STATUS_SUCCESS;
}

NTSTATUS ZwQuerySystemInformation( SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);
PVOID KernelGetModuleBase(PCHAR  pModuleName)
{
    PVOID pModuleBase = NULL;
    PULONG pSystemInfoBuffer = NULL;

    __try
    {
        NTSTATUS status = STATUS_INSUFFICIENT_RESOURCES;
        ULONG    SystemInfoBufferSize = 0;

        status = ZwQuerySystemInformation(SystemModuleInformation,
            &SystemInfoBufferSize,
            0,
            &SystemInfoBufferSize);

        if (!SystemInfoBufferSize)
            return NULL;

        pSystemInfoBuffer = (PULONG)ExAllocatePool(NonPagedPool, SystemInfoBufferSize*2);

        if (!pSystemInfoBuffer)
            return NULL;

        memset(pSystemInfoBuffer, 0, SystemInfoBufferSize*2);

        status = ZwQuerySystemInformation(SystemModuleInformation,
            pSystemInfoBuffer,
            SystemInfoBufferSize*2,
            &SystemInfoBufferSize);

        if (NT_SUCCESS(status))
        {
            PSYSTEM_MODULE_ENTRY pSysModuleEntry =
                ((PSYSTEM_MODULE_INFORMATION)(pSystemInfoBuffer))->Module;
            ULONG i;
            
            for (i = 0; i <((PSYSTEM_MODULE_INFORMATION)(pSystemInfoBuffer))->Count; i++)
            {
                if (_stricmp(pSysModuleEntry[i].ModuleName +
                             pSysModuleEntry[i].ModuleNameOffset, pModuleName) == 0)
                {
                    pModuleBase = pSysModuleEntry[i].ModuleBaseAddress;
                    break;
                }
            }
        }

    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        pModuleBase = NULL;
    }
    if(pSystemInfoBuffer) {
        ExFreePool(pSystemInfoBuffer);
    }

    return pModuleBase;
} // end KernelGetModuleBase()

PVOID RtlImageDirectoryEntryToData(PVOID Base,BOOLEAN MappedAsImage,USHORT DirectoryEntry,PULONG Size);
#define IMAGE_DIRECTORY_ENTRY_EXPORT 0
PVOID KernelGetProcAddress(char* ModuleBase, PCHAR pFunctionName)
{
    PVOID pFunctionAddress = NULL;
    
    __try
    {
        ULONG                 size = 0;
        PIMAGE_EXPORT_DIRECTORY exports =(PIMAGE_EXPORT_DIRECTORY)
            RtlImageDirectoryEntryToData(ModuleBase, TRUE, IMAGE_DIRECTORY_ENTRY_EXPORT, &size);

        char*  pAddrOfTable = (char*)((char*)exports-ModuleBase);
        PULONG apFunctionsOffset =(PULONG)(ModuleBase + exports->AddressOfFunctions);
        PULONG apNameOffsets = (PULONG)(ModuleBase + exports->AddressOfNames);
        ULONG  max_name  =exports->NumberOfNames;
        ULONG  max_func  =exports->NumberOfFunctions;
		char* psz;
        ULONG i;

        for (i = 0; i < max_name; i++)
        {
			psz = (ModuleBase + apNameOffsets[i]);
            //if (strcmp(ModuleBase + apNameOffsets[i], pFunctionName)  == 0)
            if (strstr(ModuleBase + apNameOffsets[i], pFunctionName)  != NULL)
            {
                pFunctionAddress =(PVOID)(ModuleBase + apFunctionsOffset[i]);
                break;
            }
        }
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        pFunctionAddress = NULL;
    }

    return pFunctionAddress;
} // end KernelGetProcAddress()

