#include <efi.h>
#include <efilib.h>

static CHAR16 *exampleText = L"Example EFI Application. Press any key!";

/**
 * efi_main - The entry point for the EFI application
 * @image: firmware-allocated handle that identifies the image
 * @SystemTable: EFI system table
 */
EFI_STATUS
efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *systemTable)
{
	UINTN index;
	EFI_EVENT event = systemTable->ConIn->WaitForKey;

//	SIMPLE_INPUT_INTERFACE *conIn = systemTable->ConIn;
	SIMPLE_TEXT_OUTPUT_INTERFACE *conOut = systemTable->ConOut;
	conOut->OutputString(conOut, exampleText);

	systemTable->BootServices->WaitForEvent(1, &event, &index);

	return EFI_SUCCESS;
}
