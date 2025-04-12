#include <ntddk.h>


VOID UnloadDriver(PDRIVER_OBJECT DriverObject) {
    UNREFERENCED_PARAMETER(DriverObject);
    DbgPrint("Hello World Driver Unloaded\n");
    KdPrint(("Sample driver unload called\n"));
}

extern "C"
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);
    DriverObject->DriverUnload = UnloadDriver;
    DbgPrint("Hello World Driver Loaded\n");
    KdPrint(("Sample driver Loaded called\n"));
    return STATUS_SUCCESS;
}