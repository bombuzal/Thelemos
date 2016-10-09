#include <efi.h>
#include <efilib.h>

#define ErrorCheck(actual, expected) if(actual != expected) return actual


/**
 * efi_main - The entry point for the EFI application
 * @ImageHandle: firmware-allocated handle that identifies the image
 * @SystemTable: EFI system table
 */

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_STATUS Status;
    EFI_INPUT_KEY Key;

    ST = SystemTable;
    EFI_BOOT_SERVICES *bs = ST->BootServices;
    EFI_RUNTIME_SERVICES *rs = ST->RuntimeServices;

    ST->ConOut->ClearScreen(ST->ConOut);

    Status = ST->ConOut->OutputString(ST->ConOut, L"THELEMOS\n\r");
    if (EFI_ERROR(Status))
        return Status;

    bs->SetWatchdogTimer(0, 0, 0, NULL);

    Status = ST->ConIn->Reset(ST->ConIn, FALSE);
    if (EFI_ERROR(Status))
        return Status;

    /* Load OS kernel and anything else that would change the map */

    UINTN mapSize = 0, mapKey, descriptorSize;
    EFI_MEMORY_DESCRIPTOR *memoryMap = NULL;
    UINT32 descriptorVersion;

    // Get required memory pool size for the memory map
    Status = bs->GetMemoryMap(&mapSize, memoryMap, NULL, &descriptorSize, NULL);
    ErrorCheck(Status, EFI_BUFFER_TOO_SMALL);

    /* Allocating the pool creates at least one new descriptor...
       for the chunk of memory changed to EdiLoaderData
       Not sure that UEFI fw must alloc on a memory type boundary
       if not, then two descriptor might be created */

    mapSize += 2 * descriptorSize;

    // Get a pool of memory to hold mmap
    Status = bs->AllocatePool(EfiLoaderData, mapSize, (void **)&memoryMap);
    if (EFI_ERROR(Status))
	return Status;

    // Get map
    Status = bs->GetMemoryMap(&mapSize, memoryMap, &mapKey, &descriptorSize, &descriptorVersion);
    if (EFI_ERROR(Status))
        return Status;

    while ((Status = ST->ConIn->ReadKeyStroke(ST->ConIn, &Key)) == EFI_NOT_READY) ;

    return Status;
}
