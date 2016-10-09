#include <efi.h>
#include <efilib.h>

static EFI_GUID GraphicsOutputProtocolGUID = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;


void printInt(SIMPLE_TEXT_OUTPUT_INTERFACE *conOut, int value) {
	CHAR16 out[32];
	CHAR16 *ptr = out;
	if (value == 0) {
		conOut->OutputString(conOut, L"0");
		return;
	}

	ptr += 31;
	*--ptr = 0;
	int tmp = value;// >= 0 ? value : -value; 
	
	while (tmp) {
		*--ptr = '0' + tmp % 10;
		tmp /= 10;
	}
	if (value < 0) *--ptr = '-';
	conOut->OutputString(conOut, ptr);
}

/**
 * efi_main - The entry point for the EFI application
 * @image: firmware-allocated handle that identifies the image
 * @SystemTable: EFI system table
 */
EFI_STATUS
efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *systemTable)
{
	EFI_BOOT_SERVICES *bs = systemTable->BootServices;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *graphicsProtocol;

	SIMPLE_TEXT_OUTPUT_INTERFACE *conOut = systemTable->ConOut;
	EFI_EVENT event = systemTable->ConIn->WaitForKey;
	UINTN index;

	int i, modeCount;

	EFI_STATUS status = bs->LocateProtocol(&GraphicsOutputProtocolGUID, NULL, 
		(void**)&graphicsProtocol);

	if (EFI_ERROR(status) || graphicsProtocol == NULL) {
		conOut->OutputString(conOut, L"Failed to init gfx!\r\n");
		return status;
	}

	//Switch to current mode so gfx is started.
	status = graphicsProtocol->SetMode(graphicsProtocol, graphicsProtocol->Mode->Mode);
	if (EFI_ERROR(status)) {
		conOut->OutputString(conOut, L"Failed to set default mode!\r\n");
		return status;
	}

	conOut->OutputString(conOut, L"Current mode: ");
	printInt(conOut, graphicsProtocol->Mode->Mode);
	conOut->OutputString(conOut, L"\r\n");

	modeCount = graphicsProtocol->Mode->MaxMode;
	for (i = 0; i < modeCount; i++) {
		conOut->OutputString(conOut, L"\r\n");
		printInt(conOut, i);
		conOut->OutputString(conOut, L": ");
		EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
		UINTN SizeOfInfo;
		status = graphicsProtocol->QueryMode(graphicsProtocol, i, &SizeOfInfo, &info);

		if (EFI_ERROR(status)) {
			conOut->OutputString(conOut, L" Failure to query mode: ");
			printInt(conOut, status);
			continue;
		}
		printInt(conOut, info->HorizontalResolution);
		conOut->OutputString(conOut, L" x ");
		printInt(conOut, info->VerticalResolution);

		switch(info->PixelFormat) {
			case PixelRedGreenBlueReserved8BitPerColor:
				conOut->OutputString(conOut, L" RGB(R)");
				break;
			case PixelBlueGreenRedReserved8BitPerColor:
				conOut->OutputString(conOut, L" BGR(R)");
				break;
			case PixelBitMask:
				conOut->OutputString(conOut, L" BitMask ");
				printInt(conOut, info->PixelInformation.RedMask);
				conOut->OutputString(conOut, L"R ");
				printInt(conOut, info->PixelInformation.GreenMask);
				conOut->OutputString(conOut, L"G ");
				printInt(conOut, info->PixelInformation.BlueMask);
				conOut->OutputString(conOut, L"B ");
				printInt(conOut, info->PixelInformation.ReservedMask);
				conOut->OutputString(conOut, L"Reserved ");
				break;
			case PixelBltOnly:
				conOut->OutputString(conOut, L" (blt only)");
				break;
			default:
				conOut->OutputString(conOut, L" (Invalid pixel format)");
				break;
		}
		conOut->OutputString(conOut, L" Pixel per scanline: ");
		printInt(conOut, info->PixelsPerScanLine);
	}

	bs->WaitForEvent(1, &event, &index);
	return EFI_SUCCESS;
}
