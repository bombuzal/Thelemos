#
# Create a GPT HD image with fat.img as 'EFI System Partition'
#
[ -f build/BOOTX64.EFI ] || ./build.sh
[ -f build/fat.img ] || ./mkfat.sh
cd build
mkdir iso
cp fat.img iso
xorriso -as mkisofs -R -f -e fat.img -no-emul-boot -o cdimage.iso iso
cd ..
# qemu-system-x86_64 -L 3rdparty/bios/ -bios 3rdparty/OVMF.fd -cdrom build/cdimage.iso
