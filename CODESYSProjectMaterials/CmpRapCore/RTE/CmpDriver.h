
//
// Define the various device type values.  Note that values used by Microsoft
// Corporation are in the range 0-32767, and 32768-65535 are reserved for use
// by customers.
//

#define MY_CMPDRV_NAME "CmpRapCore"
#define MY_CMPDRV_NAME_L L"CmpRapCore"
#ifndef CMP_VERSION
#define CMP_VERSION		RTS_VERSION
#endif

typedef enum _SYSTEM_INFORMATION_CLASS {

SystemBasicInformation,
SystemProcessorInformation, 
SystemPerformanceInformation, 
SystemTimeOfDayInformation, 
SystemPathInformation, 
SystemProcessInformation, 
SystemCallCountInformation, 
SystemDeviceInformation, 
SystemProcessorPerformanceInformation, 
SystemFlagsInformation, 
SystemCallTimeInformation, 
SystemModuleInformation, 
SystemLocksInformation, 
SystemStackTraceInformation, 
SystemPagedPoolInformation, 
SystemNonPagedPoolInformation, 
SystemHandleInformation, 
SystemObjectInformation, 
SystemPageFileInformation, 
SystemVdmInstemulInformation, 
SystemVdmBopInformation, 
SystemFileCacheInformation, 
SystemPoolTagInformation, 
SystemInterruptInformation, 
SystemDpcBehaviorInformation, 
SystemFullMemoryInformation, 
SystemLoadGdiDriverInformation, 
SystemUnloadGdiDriverInformation, 
SystemTimeAdjustmentInformation, 
SystemSummaryMemoryInformation, 
SystemNextEventIdInformation, 
SystemEventIdsInformation, 
SystemCrashDumpInformation, 
SystemExceptionInformation, 
SystemCrashDumpStateInformation,
SystemKernelDebuggerInformation,
SystemContextSwitchInformation,
SystemRegistryQuotaInformation,
SystemExtendServiceTableInformation,
SystemPrioritySeperation,
SystemPlugPlayBusInformation,
SystemDockInformation,

} SYSTEM_INFORMATION_CLASS, *PSYSTEM_INFORMATION_CLASS;

typedef struct _SYSTEM_MODULE_INFORMATION_ENTRY
{
    ULONG  Unknown1;
    ULONG  Unknown2;
#ifdef _WIN64
        ULONG Unknown3;
        ULONG Unknown4;
#endif
    PVOID  Base;
    ULONG  Size;
    ULONG  Flags;
    USHORT  Index;
    USHORT  NameLength;
    USHORT  LoadCount;
    USHORT  PathLength;
    CHAR  ImageName[256];
} SYSTEM_MODULE_INFORMATION_ENTRY, *PSYSTEM_MODULE_INFORMATION_ENTRY;

typedef struct _SYSTEM_MODULE_INFORMATION
{
    UINT_PTR Count;
    SYSTEM_MODULE_INFORMATION_ENTRY Module[1];
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;

typedef struct _SYSTEM_MODULE_ENTRY
{
    UINT_PTR  Unused;
    UINT_PTR  Always0;
    PVOID  ModuleBaseAddress;
    ULONG  ModuleSize;
    ULONG  Unknown;
    ULONG  ModuleEntryIndex;
    USHORT ModuleNameLength;
    USHORT ModuleNameOffset;
    CHAR   ModuleName [256];
} SYSTEM_MODULE_ENTRY, * PSYSTEM_MODULE_ENTRY;

typedef struct _LDR_DATA_TABLE_ENTRY {
    LIST_ENTRY     LoadOrder;
    LIST_ENTRY     MemoryOrder;
    LIST_ENTRY     InitializationOrder;
    PVOID          ModuleBaseAddress;
    PVOID          EntryPoint;
    ULONG          ModuleSize;
    UNICODE_STRING FullModuleName;
    UNICODE_STRING ModuleName;
    ULONG          Flags;
    USHORT         LoadCount;
    USHORT         TlsIndex;
    union {
        LIST_ENTRY Hash;
        struct {
            PVOID SectionPointer;
            ULONG CheckSum;
        };
    };
    ULONG   TimeStamp;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

typedef struct _PEB_LDR_DATA {
    ULONG          Length;
    BOOLEAN        Initialized;
    HANDLE         SsHandle;
    LIST_ENTRY     LoadOrder;
    LIST_ENTRY     MemoryOrder;
    LIST_ENTRY     InitializationOrder;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _IMAGE_EXPORT_DIRECTORY {
    ULONG   Characteristics;
    ULONG   TimeDateStamp;
    unsigned short    MajorVersion;
    unsigned short    MinorVersion;
    ULONG   Name;
    ULONG   Base;
    ULONG   NumberOfFunctions;
    ULONG   NumberOfNames;
    ULONG   AddressOfFunctions;
    ULONG   AddressOfNames;
    unsigned short* *AddressOfNameOrdinals;
} IMAGE_EXPORT_DIRECTORY, *PIMAGE_EXPORT_DIRECTORY;

