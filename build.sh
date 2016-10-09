mkdir build
cd build

x86_64-w64-mingw32-gcc -ffreestanding -I../3rdparty/gnu-efi-3.0.4/inc -I../3rdparty/gnu-efi-3.0.4/inc/x86_64 \
	-I../3rdparty/gnu-efi-3.0.4/inc/protocol -c -o efimain.o ../src/efi/efimain.c

x86_64-w64-mingw32-gcc -ffreestanding -I../3rdparty/gnu-efi-3.0.4/inc -I../3rdparty/gnu-efi-3.0.4/inc/x86_64 \
        -I../3rdparty/gnu-efi-3.0.4/inc/protocol -c -o data.o ../src/efi/data.c

x86_64-w64-mingw32-gcc -nostdlib -Wl,-dll -shared -Wl,--subsystem,10 -e efi_main -o BOOTX64.EFI \
	efimain.o data.o -lgcc

## Subsystem 10 specifies an EFI application

cd ..
