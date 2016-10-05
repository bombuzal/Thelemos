#
# Boot our GPT HD
#

qemu-system-x86_64 -L 3rdparty/bios/ -bios 3rdparty/bios/OVMF.fd -hda build/hdimage.bin
