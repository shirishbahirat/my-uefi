INCLUDE_DIRS += /usr/include

CFLAGS := \
	-ffreestanding -DDEBUG -ggdb -O0 -MMD -mno-red-zone -std=c11 \
	-target x86_64-unknown-windows -Wall -Werror -pedantic 
LDFLAGS_RUNTIME_DRIVER := -flavor link -subsystem:efi_runtime_driver -entry:efi_main
LDFLAGS_APPLICATION := -flavor link -subsystem:efi_application -entry:efi_main
LDFLAGS_BOOT_DRIVER := -flavor link -subsystem:efi_boot_service_driver -entry:efi_main

CFLAGS_KERNEL := -ffreestanding -DDEBUG -ggdb -O0 -MMD -mno-red-zone -std=c11 -target x86_64-unknown-gnu -Wall -Werror -pedantic
LDFLAGS_KERNEL := -flavor ld -e main

# 10 = EFI application
# 11 = EFI boot service driver
# 12 = EFI runtime driver

CFLAGS += $(INCLUDE_DIRS:%=-I%)

CC := clang
LD := ld.lld
ARCH ?= x86-64

SRCS := test_main.c log.c debug.c driver_test.c \
	protocol_test.c application_test.c pei_driver.c \
	hello_world.c dyn_pcd.c loader.c

default: all

%o: %c
	$(CC) $(LDFLAGS) -c $< -o $o

kernel.elf: kernel.c
	$(CC) $(CFLAGS_KERNEL) -c $< -o kernel.o
	$(LD) $(KERNEL_LDFLAGS) kernel.o -o $@

test.efi: test_main.o log.o debug.o
	$(LD) $(LDFLAGS_APPLICATION) $^ -out:$@

application.efi: application_test.o log.o debug.o
	$(LD) $(LDFLAGS_APPLICATION) $^ -out:$@

driver.efi: driver_test.o log.o debug.o
	$(LD) $(LDFLAGS_BOOT_DRIVER) $^ -out:$@

nvme.efi: pei_driver.o log.o debug.o
	$(LD) $(LDFLAGS_BOOT_DRIVER) $^ -out:$@

protocol.efi: protocol_test.o log.o debug.o
	$(LD) $(LDFLAGS_RUNTIME_DRIVER) $^ -out:$@

hello_world.efi: hello_world.o log.o debug.o
	$(LD) $(LDFLAGS_APPLICATION) $^ -out:$@

dyn_pcd.efi: dyn_pcd.o log.o debug.o
	$(LD) $(LDFLAGS_APPLICATION) $^ -out:$@

boot.efi: loader.o log.o debug.o
	$(LD) $(LDFLAGS_APPLICATION) $^ -out:$@

-include $(SRCS:.c=.d)

clean:
	rm -f *.o *.efi *.lib *.d *.elf

all: test.efi driver.efi protocol.efi \
	application.efi nvme.efi hello_world.efi \
	dyn_pcd.efi kernel.elf boot.efi

.PHONY: clean all
