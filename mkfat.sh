#
# Create a FAT filesystem image
#

dd if=/dev/zero of=build/fat.img bs=1k count=1440
mformat -i build/fat.img -f 1440 ::
mmd -i build/fat.img ::/EFI
mmd -i build/fat.img ::/EFI/BOOT
mcopy -i build/fat.img build/BOOTX64.EFI ::/EFI/BOOT
