####
# Install prerequisites for testing Thelemos::EFI
####


## X64 / IA32
arch=X64

## mkgpt / r660 || latest
mkgpt="r660"
mkgpturl="http://www.tysos.org/files/efi/mkgpt-$mkgpt.tar.bz2"

## GNU-EFI / 3.0.4
gnuefi="3.0.4"
gnuefiurl="http://sourceforge.mirrorservice.org/g/gn/gnu-efi/gnu-efi-$gnuefi.tar.bz2"

## OVMF / r15214
ovmf="r15214"
ovmfurl="http://sourceforge.mirrorservice.org/e/ed/edk2/OVMF/OVMF-X64-$ovmf.zip"


sudo apt-get install build-essential qemu binutils-mingw-w64\
	gcc-mingw-w64 xorriso mtools

cd 3rdparty

wget -N -q "$mkgpturl"
tar jxf mkgpt-$mkgpt.tar.bz2
cd mkgpt && ./configure && make && sudo make install && cd ..

wget -N -q "$gnuefiurl"
tar jxf gnu-efi-3.0.4.tar.bz2

wget -N -q "http://sourceforge.mirrorservice.org/e/ed/edk2/OVMF/OVMF-$arch-$ovmf.zip"
unzip -o -q OVMF-X64-$ovmf.zip OVMF.fd
mkdir bios
mv OVMF.fs bios/
cd ..

## test:
# qemu-system-x86_64 -L efi/ -bios 3rdparty/OVMF.fd

cp 3rdparty/gnu-efi-$gnuefi/lib/data.c src/
cp 3rdparty/gnu-efi-$gnuefi/lib/lib.h src/


# We only want the GUIDs and header values from gnu-efi
# Remove these references to avoid unnecessary dependencies
sed -ri 's/LibStubStriCmp|LibStubMetaiMatch|LibStubStrLwrUpr/NULL/g' src/data.c

