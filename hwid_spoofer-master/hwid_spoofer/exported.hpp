#pragma once
#include <ntifs.h>
#include <classpnp.h>

typedef struct _LDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

extern "C" __declspec( dllimport ) PLIST_ENTRY NTAPI PsLoadedModuleList;
extern "C" __declspec( dllimport ) POBJECT_TYPE* IoDriverObjectType;
extern "C" __declspec( dllimport ) NTSTATUS NTAPI ObReferenceObjectByName( PUNICODE_STRING, ULONG, PACCESS_STATE, ACCESS_MASK, POBJECT_TYPE, KPROCESSOR_MODE, PVOID OPTIONAL, PVOID* );