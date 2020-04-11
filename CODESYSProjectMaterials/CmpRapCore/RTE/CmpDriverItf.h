/*
Copyright by 3S Smart Software Solutions GmbH, 87439 Kempten, Germany
Author: A. Fauter
*/
#ifndef _CMP_DRV_ITF_H_
#define _CMP_DRV_ITF_H_

#define DRV_CMD_INVALID_CMD	0
#define DRV_CMD_VERSIONCHECK 2

#define FILE_DEVICE_RTIODRIVERcommon  0x00008123

//Use this for ctrlcodes only known in this driver.
#define FILE_DEVICE_RTIODRIVERthis  0x00008010

#define IOCTL_INDEX  0x800

#define CTL(i)	CTL_CODE(FILE_DEVICE_RTIODRIVER, IOCTL_INDEX + (i), METHOD_BUFFERED, FILE_ANY_ACCESS)
#define CTLcommon(i)	CTL_CODE(FILE_DEVICE_RTIODRIVERcommon, IOCTL_INDEX + (i), METHOD_BUFFERED, FILE_ANY_ACCESS)

#ifndef _RTE_COMMON_H

#define IOCTL_DIRECT_COMMAND CTLcommon(3)
//a directcommand may be sent in future, for any case, by an application like RTService, to the driver.
//these commands have to be kept unique for 3S-products.

//
// Macro definition for defining IOCTL and FSCTL function control codes.  Note
// that function codes 0-2047 are reserved for Microsoft Corporation, and
// 2048-4095 are reserved for customers.
//

#define RTS_CUSTOM_SERVICES 255

#define SZREGKEY_ROOT_MACHINE		"\\Registry\\Machine"
#define SZREGKEY_DRIVER             SZREGKEY_ROOT_MACHINE "\\SYSTEM\\CurrentControlSet\\Services\\"MY_CMPDRV_NAME
#define SZREGKEY_DRIVERPARAMS       SZREGKEY_DRIVER "\\Params"             
 
typedef struct tagAdditionalParameter
{
	UINT_PTR ulPar1;
	UINT_PTR ulPar2;
	UINT_PTR ulPar3;
	UINT_PTR ulPar4;
	UINT_PTR ulPar5;
}AdditionalParameter;

typedef struct tagDirectCommand
{
	unsigned long Cmd;
	unsigned long SubCmd;
	UINT_PTR ulRetInfo;
	AdditionalParameter adpar;
}DirectCommand;
            
//structure to pass information to the callback-function.
typedef struct _tagCallbackInfo
{
		//the security-members are made to check, whether the caller is a legal caller to the driver.
	unsigned long ulSecurityId1; // this member must be set to 0x436F4465 means CoDe
	unsigned long ulSecurityId2; // this member must be set to 0x53797320 means Sys<space>

	unsigned long ulCommand;
	unsigned long ulSubCommand;

	UINT_PTR ulCmdParam;
	void* pAdditionalCmdData; //may point to a caller-supplied, command-specific structure.
}CallbackInfo;
unsigned long ntCallback(CallbackInfo* pcbi, void* pPar);

typedef unsigned long (*PF_NT_INTERNAL_CONTROL)(CallbackInfo* pcinf, void* pv);

#define MAX_KERNEL_MODULES 20
#define MAX_EXPORTED_FUNCTIONS	40

typedef struct
{
	void* pfFct;
	char szName[80];
}ExportedFct;

typedef struct
{
	int bLoadOK;
	PF_NT_INTERNAL_CONTROL pEntryFct;
	PDRIVER_OBJECT pDrvObj;	
	char szModuleName[80];
	ExportedFct aefFctTable[MAX_EXPORTED_FUNCTIONS];
}KernelModuleDesc;

#endif /*RTE_COMMON_H_*/

//let's say 5 devices are enough for a driver.
#define DRV_MAX_DEVICES 5
//let's further say 5 interrupt-service-routines (or different service-contexts) are enough for one driver.
#define MAX_DRV_INTERRUPT_ROUTINES 5

//If caller wants to map memory, he passes a pointer to the following structure in ulPar:
#pragma pack(1)
typedef struct _tagKernelMapInfo
{
	void* pKernel;
	unsigned long ulSize;
	unsigned long ulPHandle;
	int iSuccess; //Output of the function: 0 is success, all other values are errors.
}KernelMapInfo;
#pragma pack()

#endif /*_CMP_DRV_ITF_H_*/
