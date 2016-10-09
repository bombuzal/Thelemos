#
# Create a FAT filesystem image
#
[ -f build/BOOTX64.EFI ] || ./build.sh

## Create 1.44M FAT12 image.
#
dd if=/dev/zero of=build/fat.img bs=1k count=1440
mformat -i build/fat.img -f 1440 ::
echo 
echo "WARNING:	Some UEFI implementations require FAT32 (-F)"
echo "		This increases our image to a minimum of 32 MB[-4.5K] (65525 clusters, 512b sectors)"
echo "		OVMF does not have this limitation so the above is fine for testing."
echo
mmd -i build/fat.img ::/EFI
mmd -i build/fat.img ::/EFI/BOOT
mcopy -i build/fat.img build/BOOTX64.EFI ::/EFI/BOOT
echo DONE. ./run-usb.sh to test.
