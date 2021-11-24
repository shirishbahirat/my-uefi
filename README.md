# my-uefi
uefi-test code


compile - make

to run the uefi-emulator

qemu-system-x86_64 -drive if=pflash,format=raw,file=OVMF.fd -drive format=raw,file=fat:rw:. -net none -nographic
