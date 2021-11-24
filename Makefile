CFLAGS := \
	-ffreestanding -DDEBUG -ggdb -O0 -MMD -mno-red-zone -std=c11 \
	-target x86_64-unknown-windows -Wall -Werror -pedantic 
LDFLAGS := -flavor link -subsystem:efi_application -entry:efi_main

CC := clang -I /usr/include
LD := ld.lld
ARCH ?= x86-64

SRCS := test_main.c

default: all

%o: %c
	$(CC) $(LDFLAGS) -c $< -o $o


test.efi: test_main.o
	$(LD) $(LDFLAGS) $^ -out:$@

-include $(SRCS:.c=.d)

clean:
	rm -f *.o *.efi *.lib *.d

all: test.efi

.PHONY: clean all
