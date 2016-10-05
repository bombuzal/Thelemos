#
# Create a GPT HD image with fat.img as 'EFI System Partition'
#

mkgpt -o build/hdimage.bin --image-size 4096 --part build/fat.img --type system
