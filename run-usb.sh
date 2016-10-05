#
# Run BOOTX64.EFI from fat.img as a USB flash image
#

qemu-system-x86_64 -L 3rdparty/bios/ -bios 3rdparty/bios/OVMF.fd -usb -usbdevice disk::build/fat.img
