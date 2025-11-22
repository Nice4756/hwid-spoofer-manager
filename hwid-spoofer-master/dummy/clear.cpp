#include "clear.h"
#include "ntstrsafe.h"

#define POOLTAG '6DV7'

struct piddbcache
{
	LIST_ENTRY		List;
	UNICODE_STRING	DriverName;
	ULONG			TimeDateStamp;
	NTSTATUS		LoadStatus;
	char			_0x0028[16]; // data from the shim engine, or uninitialized memory for custom drivers
};

typedef struct _RTL_PROCESS_MODULE_INFORMATION
{
	HANDLE Section;
	PVOID MappedBase;
	PVOID ImageBase;
	ULONG ImageSize;
	ULONG Flags;
	USHORT LoadOrderIndex;
	USHORT InitOrderIndex;
	USHORT LoadCount;
	USHORT OffsetToFileName;
	CHAR  FullPathName[256];
} RTL_PROCESS_MODULE_INFORMATION, * PRTL_PROCESS_MODULE_INFORMATION;

typedef struct _RTL_PROCESS_MODULES
{
	ULONG NumberOfModules;
	RTL_PROCESS_MODULE_INFORMATION Modules[1];
} RTL_PROCESS_MODULES, * PRTL_PROCESS_MODULES;


typedef enum _SYSTEM_INFORMATION_CLASS
{
	SystemBasicInformation = 0x0,
	SystemProcessorInformation = 0x1,
	SystemPerformanceInformation = 0x2,
	SystemTimeOfDayInformation = 0x3,
	SystemPathInformation = 0x4,
	SystemProcessInformation = 0x5,
	SystemCallCountInformation = 0x6,
	SystemDeviceInformation = 0x7,
	SystemProcessorPerformanceInformation = 0x8,
	SystemFlagsInformation = 0x9,
	SystemCallTimeInformation = 0xa,
	SystemModuleInformation = 0xb,
	SystemLocksInformation = 0xc,
	SystemStackTraceInformation = 0xd,
	SystemPagedPoolInformation = 0xe,
	SystemNonPagedPoolInformation = 0xf,
	SystemHandleInformation = 0x10,
	SystemObjectInformation = 0x11,
	SystemPageFileInformation = 0x12,
	SystemVdmInstemulInformation = 0x13,
	SystemVdmBopInformation = 0x14,
	SystemFileCacheInformation = 0x15,
	SystemPoolTagInformation = 0x16,
	SystemInterruptInformation = 0x17,
	SystemDpcBehaviorInformation = 0x18,
	SystemFullMemoryInformation = 0x19,
	SystemLoadGdiDriverInformation = 0x1a,
	SystemUnloadGdiDriverInformation = 0x1b,
	SystemTimeAdjustmentInformation = 0x1c,
	SystemSummaryMemoryInformation = 0x1d,
	SystemMirrorMemoryInformation = 0x1e,
	SystemPerformanceTraceInformation = 0x1f,
	SystemObsolete0 = 0x20,
	SystemExceptionInformation = 0x21,
	SystemCrashDumpStateInformation = 0x22,
	SystemKernelDebuggerInformation = 0x23,
	SystemContextSwitchInformation = 0x24,
	SystemRegistryQuotaInformation = 0x25,
	SystemExtendServiceTableInformation = 0x26,
	SystemPrioritySeperation = 0x27,
	SystemVerifierAddDriverInformation = 0x28,
	SystemVerifierRemoveDriverInformation = 0x29,
	SystemProcessorIdleInformation = 0x2a,
	SystemLegacyDriverInformation = 0x2b,
	SystemCurrentTimeZoneInformation = 0x2c,
	SystemLookasideInformation = 0x2d,
	SystemTimeSlipNotification = 0x2e,
	SystemSessionCreate = 0x2f,
	SystemSessionDetach = 0x30,
	SystemSessionInformation = 0x31,
	SystemRangeStartInformation = 0x32,
	SystemVerifierInformation = 0x33,
	SystemVerifierThunkExtend = 0x34,
	SystemSessionProcessInformation = 0x35,
	SystemLoadGdiDriverInSystemSpace = 0x36,
	SystemNumaProcessorMap = 0x37,
	SystemPrefetcherInformation = 0x38,
	SystemExtendedProcessInformation = 0x39,
	SystemRecommendedSharedDataAlignment = 0x3a,
	SystemComPlusPackage = 0x3b,
	SystemNumaAvailableMemory = 0x3c,
	SystemProcessorPowerInformation = 0x3d,
	SystemEmulationBasicInformation = 0x3e,
	SystemEmulationProcessorInformation = 0x3f,
	SystemExtendedHandleInformation = 0x40,
	SystemLostDelayedWriteInformation = 0x41,
	SystemBigPoolInformation = 0x42,
	SystemSessionPoolTagInformation = 0x43,
	SystemSessionMappedViewInformation = 0x44,
	SystemHotpatchInformation = 0x45,
	SystemObjectSecurityMode = 0x46,
	SystemWatchdogTimerHandler = 0x47,
	SystemWatchdogTimerInformation = 0x48,
	SystemLogicalProcessorInformation = 0x49,
	SystemWow64SharedInformationObsolete = 0x4a,
	SystemRegisterFirmwareTableInformationHandler = 0x4b,
	SystemFirmwareTableInformation = 0x4c,
	SystemModuleInformationEx = 0x4d,
	SystemVerifierTriageInformation = 0x4e,
	SystemSuperfetchInformation = 0x4f,
	SystemMemoryListInformation = 0x50,
	SystemFileCacheInformationEx = 0x51,
	SystemThreadPriorityClientIdInformation = 0x52,
	SystemProcessorIdleCycleTimeInformation = 0x53,
	SystemVerifierCancellationInformation = 0x54,
	SystemProcessorPowerInformationEx = 0x55,
	SystemRefTraceInformation = 0x56,
	SystemSpecialPoolInformation = 0x57,
	SystemProcessIdInformation = 0x58,
	SystemErrorPortInformation = 0x59,
	SystemBootEnvironmentInformation = 0x5a,
	SystemHypervisorInformation = 0x5b,
	SystemVerifierInformationEx = 0x5c,
	SystemTimeZoneInformation = 0x5d,
	SystemImageFileExecutionOptionsInformation = 0x5e,
	SystemCoverageInformation = 0x5f,
	SystemPrefetchPatchInformation = 0x60,
	SystemVerifierFaultsInformation = 0x61,
	SystemSystemPartitionInformation = 0x62,
	SystemSystemDiskInformation = 0x63,
	SystemProcessorPerformanceDistribution = 0x64,
	SystemNumaProximityNodeInformation = 0x65,
	SystemDynamicTimeZoneInformation = 0x66,
	SystemCodeIntegrityInformation = 0x67,
	SystemProcessorMicrocodeUpdateInformation = 0x68,
	SystemProcessorBrandString = 0x69,
	SystemVirtualAddressInformation = 0x6a,
	SystemLogicalProcessorAndGroupInformation = 0x6b,
	SystemProcessorCycleTimeInformation = 0x6c,
	SystemStoreInformation = 0x6d,
	SystemRegistryAppendString = 0x6e,
	SystemAitSamplingValue = 0x6f,
	SystemVhdBootInformation = 0x70,
	SystemCpuQuotaInformation = 0x71,
	SystemNativeBasicInformation = 0x72,
	SystemErrorPortTimeouts = 0x73,
	SystemLowPriorityIoInformation = 0x74,
	SystemBootEntropyInformation = 0x75,
	SystemVerifierCountersInformation = 0x76,
	SystemPagedPoolInformationEx = 0x77,
	SystemSystemPtesInformationEx = 0x78,
	SystemNodeDistanceInformation = 0x79,
	SystemAcpiAuditInformation = 0x7a,
	SystemBasicPerformanceInformation = 0x7b,
	SystemQueryPerformanceCounterInformation = 0x7c,
	SystemSessionBigPoolInformation = 0x7d,
	SystemBootGraphicsInformation = 0x7e,
	SystemScrubPhysicalMemoryInformation = 0x7f,
	SystemBadPageInformation = 0x80,
	SystemProcessorProfileControlArea = 0x81,
	SystemCombinePhysicalMemoryInformation = 0x82,
	SystemEntropyInterruptTimingInformation = 0x83,
	SystemConsoleInformation = 0x84,
	SystemPlatformBinaryInformation = 0x85,
	SystemThrottleNotificationInformation = 0x86,
	SystemHypervisorProcessorCountInformation = 0x87,
	SystemDeviceDataInformation = 0x88,
	SystemDeviceDataEnumerationInformation = 0x89,
	SystemMemoryTopologyInformation = 0x8a,
	SystemMemoryChannelInformation = 0x8b,
	SystemBootLogoInformation = 0x8c,
	SystemProcessorPerformanceInformationEx = 0x8d,
	SystemSpare0 = 0x8e,
	SystemSecureBootPolicyInformation = 0x8f,
	SystemPageFileInformationEx = 0x90,
	SystemSecureBootInformation = 0x91,
	SystemEntropyInterruptTimingRawInformation = 0x92,
	SystemPortableWorkspaceEfiLauncherInformation = 0x93,
	SystemFullProcessInformation = 0x94,
	SystemKernelDebuggerInformationEx = 0x95,
	SystemBootMetadataInformation = 0x96,
	SystemSoftRebootInformation = 0x97,
	SystemElamCertificateInformation = 0x98,
	SystemOfflineDumpConfigInformation = 0x99,
	SystemProcessorFeaturesInformation = 0x9a,
	SystemRegistryReconciliationInformation = 0x9b,
	MaxSystemInfoClass = 0x9c,
} SYSTEM_INFORMATION_CLASS;

extern "C"
NTSTATUS
NTAPI
ZwQuerySystemInformation(
	IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
	OUT PVOID SystemInformation,
	IN ULONG SystemInformationLength,
	OUT PULONG ReturnLength OPTIONAL
);

template <typename t = void*>
t find_pattern(void* start, size_t length, const char* pattern, const char* mask) {
	const auto data = static_cast<const char*>(start);
	const auto pattern_length = strlen(mask);

	for (size_t i = 0; i <= length - pattern_length; i++)
	{
		bool accumulative_found = true;

		for (size_t j = 0; j < pattern_length; j++)
		{
			if (!MmIsAddressValid(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(data) + i + j)))
			{
				accumulative_found = false;
				break;
			}

			if (data[i + j] != pattern[j] && mask[j] != '?')
			{
				accumulative_found = false;
				break;
			}
		}

		if (accumulative_found)
		{
			return (t)(reinterpret_cast<uintptr_t>(data) + i);
		}
	}

	return (t)nullptr;
}

uintptr_t dereference(uintptr_t address, unsigned int offset) {
	if (address == 0)
		return 0;

	return address + (int)((*(int*)(address + offset) + offset) + sizeof(int));
}

BOOLEAN bDataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return 0;

	return (*szMask) == 0;
}

UINT64 FindPattern(UINT64 dwAddress, UINT64 dwLen, BYTE* bMask, char* szMask)
{
	for (UINT64 i = 0; i < dwLen; i++)
		if (bDataCompare((BYTE*)(dwAddress + i), bMask, szMask))
			return (UINT64)(dwAddress + i);

	return 0;
}

extern "C" BOOLEAN CleanUnloadedDrivers()
{
	ULONG bytes = 0;
	NTSTATUS status = ZwQuerySystemInformation(SystemModuleInformation, 0, bytes, &bytes);

	if (!bytes)
	{
		return FALSE;
	}

	PRTL_PROCESS_MODULES modules = (PRTL_PROCESS_MODULES)ExAllocatePoolWithTag(NonPagedPool, bytes, POOLTAG);

	status = ZwQuerySystemInformation(SystemModuleInformation, modules, bytes, &bytes);

	if (!NT_SUCCESS(status))
	{
		return FALSE;
	}

	PRTL_PROCESS_MODULE_INFORMATION module = modules->Modules;
	UINT64 ntoskrnlBase = 0, ntoskrnlSize = 0;

	for (ULONG i = 0; i < modules->NumberOfModules; i++)
	{
		if (!strcmp((char*)module[i].FullPathName, "\\SystemRoot\\system32\\ntoskrnl.exe"))
		{
			ntoskrnlBase = (UINT64)module[i].ImageBase;
			ntoskrnlSize = (UINT64)module[i].ImageSize;
			break;
		}
	}

	if (modules)
		ExFreePoolWithTag(modules, 0);

	if (ntoskrnlBase <= 0)
	{
		return FALSE;
	}

	// NOTE: 4C 8B ? ? ? ? ? 4C 8B C9 4D 85 ? 74 + 3] + current signature address = MmUnloadedDrivers
	UINT64 mmUnloadedDriversPtr = FindPattern((UINT64)ntoskrnlBase, (UINT64)ntoskrnlSize, (BYTE*)"\x4C\x8B\x00\x00\x00\x00\x00\x4C\x8B\xC9\x4D\x85\x00\x74", "xx?????xxxxx?x");

	if (!mmUnloadedDriversPtr)
	{
		return FALSE;
	}

	UINT64 mmUnloadedDrivers = (UINT64)((PUCHAR)mmUnloadedDriversPtr + *(PULONG)((PUCHAR)mmUnloadedDriversPtr + 3) + 7);
	UINT64 bufferPtr = *(UINT64*)mmUnloadedDrivers;

	// NOTE: 0x7D0 is the size of the MmUnloadedDrivers array for win 7 and above
	PVOID newBuffer = ExAllocatePoolWithTag(NonPagedPoolNx, 0x7D0, POOLTAG);

	if (!newBuffer)
		return FALSE;

	memset(newBuffer, 0, 0x7D0);

	// NOTE: replace MmUnloadedDrivers
	*(UINT64*)mmUnloadedDrivers = (UINT64)newBuffer;

	// NOTE: clean the old buffer
	ExFreePoolWithTag((PVOID)bufferPtr, POOLTAG);

	return TRUE;
}

extern "C" void clean_piddb_cache() {
	ULONG bytes = 0;
	NTSTATUS status = ZwQuerySystemInformation(SystemModuleInformation, 0, bytes, &bytes);

	PRTL_PROCESS_MODULES modules = (PRTL_PROCESS_MODULES)ExAllocatePoolWithTag(NonPagedPool, bytes, POOLTAG); // 'ENON'

	status = ZwQuerySystemInformation(SystemModuleInformation, modules, bytes, &bytes);

	PRTL_PROCESS_MODULE_INFORMATION module = modules->Modules;
	UINT64 ntoskrnlBase = 0, ntoskrnlSize = 0;

	for (ULONG i = 0; i < modules->NumberOfModules; i++)
	{
		if (!strcmp((char*)module[i].FullPathName, "\\SystemRoot\\system32\\ntoskrnl.exe"))
		{
			ntoskrnlBase = (UINT64)module[i].ImageBase;
			ntoskrnlSize = (UINT64)module[i].ImageSize;
			break;
		}
	}

	if (modules)
		ExFreePoolWithTag(modules, 0);

	PRTL_AVL_TABLE PiDDBCacheTable;
	PiDDBCacheTable = (PRTL_AVL_TABLE)dereference(find_pattern<uintptr_t>((void*)ntoskrnlBase, ntoskrnlSize, "\x48\x8D\x0D\x00\x00\x00\x00\x4C\x89\x35\x00\x00\x00\x00\x49\x8B\xE9", "xxx????xxx????xxx"), 3);

	if (!PiDDBCacheTable)
	{
		PiDDBCacheTable = (PRTL_AVL_TABLE)dereference(find_pattern<uintptr_t>((void*)ntoskrnlBase, ntoskrnlSize, "\x48\x8D\x0D\x00\x00\x00\x00\x4C\x89\x35\x00\x00\x00\x00\xBB\x00\x00\x00\x00", "xxx????xxx????x????"), 3);

		if (!PiDDBCacheTable)
		{
		
		}
		else
		{
			uintptr_t entry_address = uintptr_t(PiDDBCacheTable->BalancedRoot.RightChild) + sizeof(RTL_BALANCED_LINKS);
			piddbcache* entry = (piddbcache*)(entry_address);

			/*capcom.sys(drvmap) : 0x57CD1415 iqvw64e.sys(kdmapper) : 0x5284EAC3*/
			if (entry->TimeDateStamp == 0x57CD1415 || entry->TimeDateStamp == 0x5284EAC3) {
				RemoveEntryList(&entry->List);
				RtlDeleteElementGenericTableAvl(PiDDBCacheTable, entry);
			}

			ULONG count = 0;

			for (auto link = entry->List.Flink; link != entry->List.Blink; link = link->Flink, count++)
			{
				piddbcache* cache_entry = (piddbcache*)(link);

				if (cache_entry->TimeDateStamp == 0x57CD1415 || cache_entry->TimeDateStamp == 0x5284EAC3) {
					RemoveEntryList(&cache_entry->List);
					RtlDeleteElementGenericTableAvl(PiDDBCacheTable, cache_entry);
				}
				//DbgPrint("cache_entry count: %lu name: %wZ \t\t stamp: %x\n", count, cache_entry->DriverName, cache_entry->TimeDateStamp);
			}
		}
	}
	else
	{
		uintptr_t entry_address = uintptr_t(PiDDBCacheTable->BalancedRoot.RightChild) + sizeof(RTL_BALANCED_LINKS);
		piddbcache* entry = (piddbcache*)(entry_address);

		/*capcom.sys(drvmap) : 0x57CD1415 iqvw64e.sys(kdmapper) : 0x5284EAC3*/
		if (entry->TimeDateStamp == 0x57CD1415 || entry->TimeDateStamp == 0x5284EAC3) {
			RemoveEntryList(&entry->List);
			RtlDeleteElementGenericTableAvl(PiDDBCacheTable, entry);
		}

		ULONG count = 0;

		for (auto link = entry->List.Flink; link != entry->List.Blink; link = link->Flink, count++)
		{
			piddbcache* cache_entry = (piddbcache*)(link);

			if (cache_entry->TimeDateStamp == 0x57CD1415 || cache_entry->TimeDateStamp == 0x5284EAC3) {
				RemoveEntryList(&cache_entry->List);
				RtlDeleteElementGenericTableAvl(PiDDBCacheTable, cache_entry);
			}
			//DbgPrint("cache_entry count: %lu name: %wZ \t\t stamp: %x\n", count, cache_entry->DriverName, cache_entry->TimeDateStamp);
		}
	}
}

typedef struct _INQUIRYDATA
{
	union
	{
		struct
		{
			CHAR DeviceType : 5;
			CHAR DeviceTypeQualifier : 3;
			CHAR DeviceTypeModifier : 7;
			CHAR RemovableMedia : 1;
			CHAR ANSIVersion : 3;
			CHAR ECMAVersion : 3;
			CHAR ISOVersion : 2;
			CHAR ResponseDataFormat : 4;
			CHAR HiSupport : 1;
			CHAR NormACA : 1;
			CHAR TerminateTask : 1;
			CHAR AERC : 1;
		} s0;

		struct
		{
			BYTE gap0[2];
			CHAR Versions;
		} s1;
	} u0;

	CHAR AdditionalLength;

	union
	{
		CHAR Reserved;
		struct
		{
			CHAR Protect : 1;
			CHAR Reserved_1 : 2;
			CHAR ThirdPartyCoppy : 1;
			CHAR TPGS : 2;
			CHAR ACC : 1;
			CHAR SCCS : 1;
			CHAR Addr16 : 1;
			CHAR Addr32 : 1;
			CHAR AckReqQ : 1;
			CHAR MediumChanger : 1;
			CHAR MultiPort : 1;
			CHAR ReservedBit2 : 1;
			CHAR EnclosureServices : 1;
			CHAR ReservedBit3 : 1;
			CHAR SoftReset : 1;
			CHAR CommandQueue : 1;
			CHAR TransferDisable : 1;
			CHAR LinkedCommands : 1;
			CHAR Synchronous : 1;
			CHAR Wide16Bit : 1;
			CHAR Wide32Bit : 1;
			CHAR RelativeAddressing : 1;
		} s0;
	} u1;

	CHAR VendorId[8];
	CHAR ProductId[16];
	CHAR ProductRevisionLevel[4];
	CHAR VendorSpecific[20];
	CHAR Reserved3[2];
	USHORT VersionDescriptors[8];
	CHAR Reserved4[30];
} INQUIRYDATA, * PINQUIRYDATA;

// 1903
typedef struct _VendorInfo1903
{
	char pad_0x0000[0x8];
	char Info[64];
} VendorInfo1903;

typedef struct _HDD_EXTENSION1903
{
	char pad_0x0000[0x68];
	VendorInfo1903* pVendorInfo;
	char pad_0x0068[0x8];
	char* pHDDSerial;
} *PHDD_EXTENSION1903;

typedef struct _VendorInfo1809
{
	char pad_0x0000[0x8];
	char Info[64];
} VendorInfo1809;

typedef struct _HDD_EXTENSION1809
{
	char pad_0x0000[0x60];
	VendorInfo1809* pVendorInfo;
	char pad_0x0068[0x8];
	INQUIRYDATA* InquiryData;
	char* pHDDSerial;
	char pad_0x0078[0x30];
} *PHDD_EXTENSION1809;

// 1803
typedef struct _VendorInfo1803
{
	char pad_0x0000[0x8];
	char Info[64];
} VendorInfo1803;

typedef struct _HDD_EXTENSION1803
{
	char pad_0x0000[0x60];
	VendorInfo1803* pVendorInfo;
	char pad_0x0068[0x8];
	char* pHDDSerial;
	char pad_0x0078[0x30];
} *PHDD_EXTENSION1803;

typedef struct _VendorInfo
{
	char pad_0x0000[0x8];
	char Info[64];
} VendorInfo;

typedef struct _HDD_EXTENSION
{
	char pad_0x0000[0x60];
	VendorInfo* pVendorInfo;
	char pad_0x0068[0x8];
	char* pHDDSerial;
	char pad_0x0078[0x30];
} HDD_EXTENSION, * PHDD_EXTENSION;

typedef __int64(__fastcall* RaidUnitRegisterInterfaces1903)(PHDD_EXTENSION1903 a1);
typedef __int64(__fastcall* RaidUnitRegisterInterfaces1809)(PHDD_EXTENSION1809 a1);
typedef __int64(__fastcall* RaidUnitRegisterInterfaces1803)(PHDD_EXTENSION1803 a1);
typedef __int64(__fastcall* RaidUnitRegisterInterfaces)(PHDD_EXTENSION a1);

extern "C" NTSYSAPI ULONG RtlRandomEx(
	PULONG Seed
);

void randstring(char* randomString, size_t length) {

	static char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

	ULONG seed = KeQueryTimeIncrement();

	if (randomString)
	{
		for (int n = 0; n <= length; n++)
		{
			int key = RtlRandomEx(&seed) % (int)(sizeof(charset) - 1);
			randomString[n] = charset[key];
		}
		//randomString[length] = '\0';
	}
}

static uintptr_t get_kernel_address(const char* name, size_t& size) {
	NTSTATUS status = STATUS_SUCCESS;
	ULONG neededSize = 0;

	ZwQuerySystemInformation(
		SystemModuleInformation,
		&neededSize,
		0,
		&neededSize
	);

	PRTL_PROCESS_MODULES pModuleList;

	pModuleList = (PRTL_PROCESS_MODULES)ExAllocatePool(NonPagedPool, neededSize);

	if (!pModuleList) {
		return 0;
	}

	status = ZwQuerySystemInformation(SystemModuleInformation,
		pModuleList,
		neededSize,
		0
	);

	ULONG i = 0;
	uintptr_t address = 0;

	for (i = 0; i < pModuleList->NumberOfModules; i++)
	{
		RTL_PROCESS_MODULE_INFORMATION mod = pModuleList->Modules[i];

		address = uintptr_t(pModuleList->Modules[i].ImageBase);
		size = uintptr_t(pModuleList->Modules[i].ImageSize);
		if (strstr(mod.FullPathName, name) != NULL)
			break;
	}

	ExFreePool(pModuleList);

	return address;
}

UINT64 storportBase;

#define MAX_HDDS 10
#define SERIAL_MAX_LENGTH 15

CHAR HDDSPOOF_BUFFER[MAX_HDDS][32] = { 0x20 };
CHAR HDDORG_BUFFER[MAX_HDDS][32] = { 0 };

extern "C" void spoof_drives()
{
	INT count = 0;

	size_t storportSize = 0;
	storportBase = get_kernel_address("storport.sys", storportSize);

	RTL_OSVERSIONINFOW osVersion = { 0 };
	osVersion.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW);
	RtlGetVersion(&osVersion);

	PDEVICE_OBJECT pObject = NULL;
	PFILE_OBJECT pFileObj = NULL;

	UNICODE_STRING DestinationString;
	RtlInitUnicodeString(&DestinationString, L"\\Device\\RaidPort0");

	NTSTATUS status = IoGetDeviceObjectPointer(&DestinationString, FILE_READ_DATA, &pFileObj, &pObject);

	PDRIVER_OBJECT pDriver = pObject->DriverObject;
	PDEVICE_OBJECT pDevice = pDriver->DeviceObject;
	
	if (osVersion.dwBuildNumber >= 18363) {
		RaidUnitRegisterInterfaces1903 pRegDevInt = find_pattern<RaidUnitRegisterInterfaces1903>((void*)storportBase, storportSize, "\x48\x89\x5C\x24\x00\x55\x56\x57\x48\x83\xEC\x50\x8B\x41\x60", "xxxx?xxxxxxxxxx");

		while (pDevice->NextDevice != NULL)
		{
			if (pDevice->DeviceType == FILE_DEVICE_DISK)
			{
				PHDD_EXTENSION1903 pDeviceHDD = (PHDD_EXTENSION1903)pDevice->DeviceExtension;

				CHAR HDDSPOOFED_TMP[32] = { 0x0 };
				randstring(HDDSPOOFED_TMP, SERIAL_MAX_LENGTH - 1);

				for (int i = 1; i <= SERIAL_MAX_LENGTH + 1; i = i + 2)
				{
					memcpy(&HDDORG_BUFFER[count][i - 1], &pDeviceHDD->pHDDSerial[i], sizeof(CHAR));
					memcpy(&HDDORG_BUFFER[count][i], &pDeviceHDD->pHDDSerial[i - 1], sizeof(CHAR));

					memcpy(&HDDSPOOF_BUFFER[count][i - 1], &HDDSPOOFED_TMP[i], sizeof(CHAR));
					memcpy(&HDDSPOOF_BUFFER[count][i], &HDDSPOOFED_TMP[i - 1], sizeof(CHAR));
				}

				RtlStringCchPrintfA(pDeviceHDD->pHDDSerial, SERIAL_MAX_LENGTH + 1, "%s", &HDDSPOOFED_TMP);

				pRegDevInt(pDeviceHDD);

				count++;
			}
			pDevice = pDevice->NextDevice;
		}
	}
	else if (osVersion.dwBuildNumber >= 18362) {
		RaidUnitRegisterInterfaces1903 pRegDevInt = find_pattern<RaidUnitRegisterInterfaces1903>((void*)storportBase, storportSize, "\x48\x89\x5C\x24\x00\x55\x56\x57\x48\x83\xEC\x50\x8B\x41\x60", "xxxx?xxxxxxxxxx");

		while (pDevice->NextDevice != NULL)
		{
			if (pDevice->DeviceType == FILE_DEVICE_DISK)
			{
				PHDD_EXTENSION1903 pDeviceHDD = (PHDD_EXTENSION1903)pDevice->DeviceExtension;

				CHAR HDDSPOOFED_TMP[32] = { 0x0 };
				randstring(HDDSPOOFED_TMP, SERIAL_MAX_LENGTH - 1);

				for (int i = 1; i <= SERIAL_MAX_LENGTH + 1; i = i + 2)
				{
					memcpy(&HDDORG_BUFFER[count][i - 1], &pDeviceHDD->pHDDSerial[i], sizeof(CHAR));
					memcpy(&HDDORG_BUFFER[count][i], &pDeviceHDD->pHDDSerial[i - 1], sizeof(CHAR));

					memcpy(&HDDSPOOF_BUFFER[count][i - 1], &HDDSPOOFED_TMP[i], sizeof(CHAR));
					memcpy(&HDDSPOOF_BUFFER[count][i], &HDDSPOOFED_TMP[i - 1], sizeof(CHAR));
				}

				RtlStringCchPrintfA(pDeviceHDD->pHDDSerial, SERIAL_MAX_LENGTH + 1, "%s", &HDDSPOOFED_TMP);

				pRegDevInt(pDeviceHDD);

				count++;
			}
			pDevice = pDevice->NextDevice;
		}
	}
	else if (osVersion.dwBuildNumber >= 17763) {
		RaidUnitRegisterInterfaces1809 pRegDevInt = find_pattern<RaidUnitRegisterInterfaces1809>((void*)storportBase, storportSize, "\x4C\x8B\xDC\x49\x89\x5B\x10\x49\x89\x6B\x18\x49\x89\x73\x20\x57\x48\x83\xEC\x50", "xxxxxxxxxxxxxxxxxxxx");

		while (pDevice->NextDevice != NULL)
		{
			if (pDevice->DeviceType == FILE_DEVICE_DISK)
			{
				PHDD_EXTENSION1809 pDeviceHDD = (PHDD_EXTENSION1809)pDevice->DeviceExtension;

				CHAR HDDSPOOFED_TMP[32] = { 0x0 };
				randstring(HDDSPOOFED_TMP, SERIAL_MAX_LENGTH - 1);

				for (int i = 1; i <= SERIAL_MAX_LENGTH + 1; i = i + 2)
				{
					memcpy(&HDDORG_BUFFER[count][i - 1], &pDeviceHDD->pHDDSerial[i], sizeof(CHAR));
					memcpy(&HDDORG_BUFFER[count][i], &pDeviceHDD->pHDDSerial[i - 1], sizeof(CHAR));

					memcpy(&HDDSPOOF_BUFFER[count][i - 1], &HDDSPOOFED_TMP[i], sizeof(CHAR));
					memcpy(&HDDSPOOF_BUFFER[count][i], &HDDSPOOFED_TMP[i - 1], sizeof(CHAR));
				}

				RtlStringCchPrintfA(pDeviceHDD->pHDDSerial, SERIAL_MAX_LENGTH + 1, "%s", &HDDSPOOFED_TMP);

				pRegDevInt(pDeviceHDD);

				count++;
			}
			pDevice = pDevice->NextDevice;
		}
	}
	else if (osVersion.dwBuildNumber >= 17134) {
		RaidUnitRegisterInterfaces1803 pRegDevInt = find_pattern<RaidUnitRegisterInterfaces1803>((void*)storportBase, storportSize, "\x4C\x8B\xDC\x49\x89\x5B\x10\x49\x89\x6B\x18\x49\x89\x73\x20\x57\x48\x83\xEC\x50", "xxxxxxxxxxxxxxxxxxxx");

		while (pDevice->NextDevice != NULL)
		{
			if (pDevice->DeviceType == FILE_DEVICE_DISK)
			{
				PHDD_EXTENSION1803 pDeviceHDD = (PHDD_EXTENSION1803)pDevice->DeviceExtension;

				CHAR HDDSPOOFED_TMP[32] = { 0x0 };
				randstring(HDDSPOOFED_TMP, SERIAL_MAX_LENGTH - 1);

				for (int i = 1; i <= SERIAL_MAX_LENGTH + 1; i = i + 2)
				{
					memcpy(&HDDORG_BUFFER[count][i - 1], &pDeviceHDD->pHDDSerial[i], sizeof(CHAR));
					memcpy(&HDDORG_BUFFER[count][i], &pDeviceHDD->pHDDSerial[i - 1], sizeof(CHAR));

					memcpy(&HDDSPOOF_BUFFER[count][i - 1], &HDDSPOOFED_TMP[i], sizeof(CHAR));
					memcpy(&HDDSPOOF_BUFFER[count][i], &HDDSPOOFED_TMP[i - 1], sizeof(CHAR));
				}

				RtlStringCchPrintfA(pDeviceHDD->pHDDSerial, SERIAL_MAX_LENGTH + 1, "%s", &HDDSPOOFED_TMP);

				pRegDevInt(pDeviceHDD);

				count++;
			}
			pDevice = pDevice->NextDevice;
		}
	}
	else if (osVersion.dwBuildNumber >= 16299) {
		RaidUnitRegisterInterfaces pRegDevInt = find_pattern<RaidUnitRegisterInterfaces>((void*)storportBase, storportSize, "\x48\x89\x5C\x24\x00\x55\x56\x57\x48\x83\xEC\x50", "xxxx?xxxxxxx");

		while (pDevice->NextDevice != NULL)
		{
			if (pDevice->DeviceType == FILE_DEVICE_DISK)
			{
				PHDD_EXTENSION pDeviceHDD = (PHDD_EXTENSION)pDevice->DeviceExtension;

				CHAR HDDSPOOFED_TMP[32] = { 0x0 };
				randstring(HDDSPOOFED_TMP, SERIAL_MAX_LENGTH - 1);

				for (int i = 1; i <= SERIAL_MAX_LENGTH + 1; i = i + 2)
				{
					memcpy(&HDDORG_BUFFER[count][i - 1], &pDeviceHDD->pHDDSerial[i], sizeof(CHAR));
					memcpy(&HDDORG_BUFFER[count][i], &pDeviceHDD->pHDDSerial[i - 1], sizeof(CHAR));

					memcpy(&HDDSPOOF_BUFFER[count][i - 1], &HDDSPOOFED_TMP[i], sizeof(CHAR));
					memcpy(&HDDSPOOF_BUFFER[count][i], &HDDSPOOFED_TMP[i - 1], sizeof(CHAR));
				}

				RtlStringCchPrintfA(pDeviceHDD->pHDDSerial, SERIAL_MAX_LENGTH + 1, "%s", &HDDSPOOFED_TMP);

				pRegDevInt(pDeviceHDD);

				count++;
			}
			pDevice = pDevice->NextDevice;
		}
	}
}
