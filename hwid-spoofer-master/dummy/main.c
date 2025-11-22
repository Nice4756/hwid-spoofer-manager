#include "stdafx.h"
#include "clear.h"

struct {
	DWORD Length;
	NIC_DRIVER Drivers[0xFF];
} NICs = { 0 };

PDRIVER_DISPATCH PartControlOriginal = 0, NsiControlOriginal = 0, GpuControlOriginal = 0;

/**** NIC ****/
NTSTATUS NICIoc(PDEVICE_OBJECT device, PIRP irp, PVOID context) {
	if (context) {
		IOC_REQUEST request = *(PIOC_REQUEST)context;
		ExFreePool(context);

		if (irp->MdlAddress) {
			SpoofBuffer(SEED, (PBYTE)MmGetSystemAddressForMdl(irp->MdlAddress), 6);
		}

		if (request.OldRoutine && irp->StackCount > 1) {
			return request.OldRoutine(device, irp, request.OldContext);
		}
	}

	return STATUS_SUCCESS;
}

NTSTATUS NICControl(PDEVICE_OBJECT device, PIRP irp) {
	for (DWORD i = 0; i < NICs.Length; ++i) {
		PNIC_DRIVER driver = &NICs.Drivers[i];

		if (driver->Original && driver->DriverObject == device->DriverObject) {
			PIO_STACK_LOCATION ioc = IoGetCurrentIrpStackLocation(irp);
			switch (ioc->Parameters.DeviceIoControl.IoControlCode) {
				case IOCTL_NDIS_QUERY_GLOBAL_STATS: {
					switch (*(PDWORD)irp->AssociatedIrp.SystemBuffer) {
						case OID_802_3_PERMANENT_ADDRESS:
						case OID_802_3_CURRENT_ADDRESS:
						case OID_802_5_PERMANENT_ADDRESS:
						case OID_802_5_CURRENT_ADDRESS:
							ChangeIoc(ioc, irp, NICIoc);
							break;
					}

					break;
				}
			}

			return driver->Original(device, irp);
		}
	}

	return STATUS_SUCCESS;
}

NTSTATUS NsiControl(PDEVICE_OBJECT device, PIRP irp) {
	PIO_STACK_LOCATION ioc = IoGetCurrentIrpStackLocation(irp);
	switch (ioc->Parameters.DeviceIoControl.IoControlCode) {
		case IOCTL_NSI_PROXY_ARP: {
			DWORD length = ioc->Parameters.DeviceIoControl.OutputBufferLength;
			NTSTATUS ret = NsiControlOriginal(device, irp);

			PNSI_PARAMS params = (PNSI_PARAMS)irp->UserBuffer;
			if (params && NSI_PARAMS_ARP == params->Type) {
				memset(irp->UserBuffer, 0, length);
			}

			return ret;
		}
	}

	return NsiControlOriginal(device, irp);
}

VOID SpoofNIC() {
	SwapControl(RTL_CONSTANT_STRING(L"\\Driver\\nsiproxy"), NsiControl, NsiControlOriginal);

	PVOID base = GetBaseAddress("ndis.sys", 0);
	
	if (!base) {
		return;
	}

	PNDIS_FILTER_BLOCK ndisGlobalFilterList = FindPatternImage(base, "\x40\x8A\xF0\x48\x8B\x05", "xxxxxx");
	
	if (ndisGlobalFilterList) {
		
		PDWORD ndisFilter_IfBlock = FindPatternImage(base, "\x48\x85\x00\x0F\x84\x00\x00\x00\x00\x00\x8B\x00\x00\x00\x00\x00\x33", "xx?xx?????x???xxx");
		
		if (ndisFilter_IfBlock) {
			DWORD ndisFilter_IfBlock_offset = *(PDWORD)((PBYTE)ndisFilter_IfBlock + 12);

			ndisGlobalFilterList = (PNDIS_FILTER_BLOCK)((PBYTE)ndisGlobalFilterList + 3);
			ndisGlobalFilterList = *(PNDIS_FILTER_BLOCK *)((PBYTE)ndisGlobalFilterList + 7 + *(PINT)((PBYTE)ndisGlobalFilterList + 3));

			DWORD count = 0;
			for (PNDIS_FILTER_BLOCK filter = ndisGlobalFilterList; filter; filter = filter->NextFilter) {
				PNDIS_IF_BLOCK block = *(PNDIS_IF_BLOCK *)((PBYTE)filter + ndisFilter_IfBlock_offset);
				if (block) {
					PWCHAR copy = SafeCopy(filter->FilterInstanceName->Buffer, MAX_PATH);
					if (copy) {
						WCHAR adapter[MAX_PATH] = { 0 };
						swprintf(adapter, L"\\Device\\%ws", TrimGUID(copy, MAX_PATH / 2));
						ExFreePool(copy);

						UNICODE_STRING name = { 0 };
						RtlInitUnicodeString(&name, adapter);

						PFILE_OBJECT file = 0;
						PDEVICE_OBJECT device = 0;

						NTSTATUS status = IoGetDeviceObjectPointer(&name, FILE_READ_DATA, &file, &device);
						if (NT_SUCCESS(status)) {
							PDRIVER_OBJECT driver = device->DriverObject;
							if (driver) {
								BOOL exists = FALSE;
								for (DWORD i = 0; i < NICs.Length; ++i) {
									if (NICs.Drivers[i].DriverObject == driver) {
										exists = TRUE;
										break;
									}
								}

								if (exists) {
									
								} else {
									PNIC_DRIVER nic = &NICs.Drivers[NICs.Length];
									nic->DriverObject = driver;

									AppendSwap(driver->DriverName, &driver->MajorFunction[IRP_MJ_DEVICE_CONTROL], NICControl, nic->Original);

									++NICs.Length;
								}
							}

							// Indirectly dereferences device object
							ObDereferenceObject(file);
						} else {
							
						}
					}

					// Current MAC
					PIF_PHYSICAL_ADDRESS_LH addr = &block->ifPhysAddress;
					SpoofBuffer(SEED, addr->Address, addr->Length);
					addr = &block->PermanentPhysAddress;
					SpoofBuffer(SEED, addr->Address, addr->Length);

					++count;
				}
			}
		} else {
			
		}
	} else {
		
	}
}

/**** SMBIOS (and boot) ****/
void SpoofSMBIOS() {
	PVOID base = GetBaseAddress("ntoskrnl.exe", 0);
	
	if (!base) {
		
		return;
	}

	PBYTE ExpBootEnvironmentInformation = FindPatternImage(base, "\x0F\x10\x05\x00\x00\x00\x00\x0F\x11\x00\x8B", "xxx????xx?x");
	
	if (ExpBootEnvironmentInformation) {
		ExpBootEnvironmentInformation = ExpBootEnvironmentInformation + 7 + *(PINT)(ExpBootEnvironmentInformation + 3);
		SpoofBuffer(SEED, ExpBootEnvironmentInformation, 16);

	
	} else {
		
	}

	PPHYSICAL_ADDRESS WmipSMBiosTablePhysicalAddress = FindPatternImage(base, "\x48\x8B\x0D\x00\x00\x00\x00\x48\x85\xC9\x74\x00\x8B\x15", "xxx????xxxx?xx");
	
	if (WmipSMBiosTablePhysicalAddress) {
		WmipSMBiosTablePhysicalAddress = (PPHYSICAL_ADDRESS)((PBYTE)WmipSMBiosTablePhysicalAddress + 7 + *(PINT)((PBYTE)WmipSMBiosTablePhysicalAddress + 3));
		memset(WmipSMBiosTablePhysicalAddress, 0, sizeof(PHYSICAL_ADDRESS));
	} else {

	}
}

/**** GPU ****/
NTSTATUS GpuControl(PDEVICE_OBJECT device, PIRP irp) {
	PIO_STACK_LOCATION ioc = IoGetCurrentIrpStackLocation(irp);
	
	switch (ioc->Parameters.DeviceIoControl.IoControlCode) {
		case IOCTL_NVIDIA_SMIL: {
			NTSTATUS ret = GpuControlOriginal(device, irp);

			PCHAR buffer = irp->UserBuffer;
			if (buffer) {
				PCHAR copy = SafeCopy(buffer, IOCTL_NVIDIA_SMIL_MAX);
				if (copy) {
					for (DWORD i = 0; i < IOCTL_NVIDIA_SMIL_MAX - 4; ++i) {
						if (0 == memcmp(copy + i, "GPU-", 4)) {
							buffer[i] = 0;
							break;
						}
					}
					ExFreePool(copy);
				}
			}
			return ret;
		}
	}

	return GpuControlOriginal(device, irp);
}

VOID SpoofGPU() {
	SwapControl(RTL_CONSTANT_STRING(L"\\Driver\\nvlddmkm"), GpuControl, GpuControlOriginal);
}

VOID DriverUnload(PDRIVER_OBJECT driver) {
	UNREFERENCED_PARAMETER(driver);

	for (DWORD i = 0; i < SWAPS.Length; ++i) {
		PSWAP s = (PSWAP)&SWAPS.Buffer[i];
		if (s->Swap && s->Original) {
			InterlockedExchangePointer(s->Swap, s->Original);
		}
	}
}

extern void spoof_drives();
extern void clean_piddb_cache();
extern BOOLEAN CleanUnloadedDrivers();

NTSTATUS DriverEntry(PDRIVER_OBJECT driver, PUNICODE_STRING registry_path) {
	UNREFERENCED_PARAMETER(registry_path);
	driver->DriverUnload = DriverUnload;

	ULONG64 time = 0;
	KeQuerySystemTime(&time);
	SEED = (DWORD)time;

	CHAR alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

	for (DWORD i = 0, l = (DWORD)strlen(SERIAL); i < l; ++i) {
		SERIAL[i] = alphabet[RtlRandomEx(&SEED) % (sizeof(alphabet) - 1)];
	}

	CleanUnloadedDrivers();
	clean_piddb_cache();

	spoof_drives();
	SpoofNIC();
	SpoofSMBIOS();
	SpoofGPU();

	return STATUS_SUCCESS;
}
