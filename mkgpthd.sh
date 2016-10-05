#
# Create a GPT HD image with fat.img as 'EFI System Partition'
#
[ -f build/BOOTX64.EFI ] || ./build.sh
[ -f build/fat.img ] || ./mkfat.sh
echo Building GPT HD image with EFI System Partition...
mkgpt -o build/hdimage.bin --image-size 4096 --part build/fat.img --type system
echo DONE
