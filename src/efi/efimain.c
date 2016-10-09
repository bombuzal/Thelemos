#include <efi.h>
#include <efilib.h>

#define ErrorCheck(actual, expected) if(actual != expected) return actual

// static EFI_GUID SimpleFSProtocolGUID = SIMPLE_FILE_SYSTEM_PROTOCOL;


EFI_STATUS EFIAPI FetchMemoryMap(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
    EFI_STATUS Status = EFI_SUCCESS;
    ST = SystemTable;
    EFI_BOOT_SERVICES *bs = ST->BootServices;
    UINTN mapSize = 0, mapKey, descriptorSize;
    EFI_MEMORY_DESCRIPTOR *memoryMap = NULL;
    UINT32 descriptorVersion;
    Status = bs->GetMemoryMap(&mapSize, memoryMap, NULL, &descriptorSize, NULL);
    ErrorCheck(Status, EFI_BUFFER_TOO_SMALL);
    mapSize += 2 * descriptorSize;

    Status = bs->AllocatePool(EfiLoaderData, mapSize, (void **)&memoryMap);
    if (EFI_ERROR(Status))
        return Status;

    Status = bs->GetMemoryMap(&mapSize, memoryMap, &mapKey, &descriptorSize, &descriptorVersion);
    if (EFI_ERROR(Status))
        return Status;

    Status = bs->ExitBootServices(ImageHandle, mapKey);
    if (EFI_ERROR(Status))
        return Status;

}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_STATUS Status;
    EFI_INPUT_KEY Key;
    EFI_LOADED_IMAGE *loaded_image = NULL;
    EFI_GUID loaded_image_protocol = LOADED_IMAGE_PROTOCOL;
//    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FsProtocol = NULL;

    ST = SystemTable;
    EFI_BOOT_SERVICES *bs = ST->BootServices;
    EFI_RUNTIME_SERVICES *rs = ST->RuntimeServices;

    ST->ConOut->ClearScreen(ST->ConOut);

    Status = bs->HandleProtocol(ImageHandle, &loaded_image_protocol, (void **) &loaded_image);
    if (EFI_ERROR(Status))
        return Status;
    ST->ConOut->OutputString(ST->ConOut, (CHAR16 *)loaded_image->FilePath);

    Status = ST->ConOut->OutputString(ST->ConOut, L"\n\rTHELEMOS\n\r");
    if (EFI_ERROR(Status))
	return Status;

    bs->SetWatchdogTimer(0, 0, 0, NULL);

    /* load OS kernel here (before FetchMemoryMap) */

//    EFI_GUID sfspGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
//    EFI_GUID sfspGuid = SIMPLE_FILE_SYSTEM_PROTOCOL;
//    EFI_HANDLE* handles = NULL;
//    UINTN handleCount = 0;
//    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* fs = NULL;
//    SIMPLE_FILE_SYSTEM_PROTOCOL* fs = NULL;
//    Status = bs->LocateProtocol (&sfspGuid, NULL, (VOID **)&fs);
//    if (EFI_ERROR(Status))
//        return Status;

/*
    Status = bs->LocateHandleBuffer(ByProtocol, 
                                   &sfspGuid, 
                                   NULL, 
                                   &handleCount, 
                                   &handles);
    for (index = 0; index < (int)handleCount; ++ index)
    {
    	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* fs = NULL;

    	efiStatus = bs->HandleProtocol(
        	handles[index],
        	&sfspGuid,
        	(void**)&fs);

*/
 /* end loading */
    Status = ST->ConOut->OutputString(ST->ConOut, L"LOADING...\n\r");
    if (EFI_ERROR(Status))
        return Status;

    Status = ST->ConIn->Reset(ST->ConIn, FALSE);
    if (EFI_ERROR(Status))
        return Status;
    while ((Status = ST->ConIn->ReadKeyStroke(ST->ConIn, &Key)) == EFI_NOT_READY) ;
 //   return Status;

/*
   EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FsProtocol = NULL;
   Status = bs->LocateProtocol (&FileSystemProtocol, NULL, (VOID **)&FsProtocol);
    if (EFI_ERROR(Status))
        return Status;
*/

// Get Memory Map
    FetchMemoryMap(ImageHandle, ST);
// handover?
    /*EFI_IMAGE_START                 StartImage;
	EFI_STATUS
	(EFIAPI *EFI_IMAGE_START) (
	    IN EFI_HANDLE                   ImageHandle,
	    OUT UINTN                       *ExitDataSize,
	    OUT CHAR16                      **ExitData  OPTIONAL
	    );
    */




/*
    Status = ST->ConIn->Reset(ST->ConIn, FALSE);
    if (EFI_ERROR(Status))
	return Status;
    while ((Status = ST->ConIn->ReadKeyStroke(ST->ConIn, &Key)) == EFI_NOT_READY) ;
    return Status;
*/
return EFI_SUCCESS;
}
