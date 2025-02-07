#include "Log.h"

EXTERN_C_START
DRIVER_INITIALIZE DriverEntry;
EXTERN_C_END

#pragma alloc_text(INIT, DriverEntry)


extern "C"
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(DriverObject);
    UNREFERENCED_PARAMETER(RegistryPath);
    ExInitializeDriverRuntime(DrvRtPoolNxOptIn);
    return STATUS_SUCCESS;
}