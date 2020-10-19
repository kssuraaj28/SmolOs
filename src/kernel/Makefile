CFLAGS?=-g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

CC:=i686-elf-gcc
LD:=i686-elf-ld

CFLAGS:=$(CFLAGS) -ffreestanding -O2 -Wall -Wextra -Werror
CPPFLAGS:=$(CPPFLAGS) -Iinclude
LDFLAGS:=$(LDFLAGS) -Tlinker.ld
LIBS:=$(LIBS) -nostdlib -lgcc 

KERNEL_SOURCES :=  \
entry.asm \
kmain.c \
desc_table/gdt.c \
desc_table/idt.c \
mem/pfa.c \
mem/paging.c \
interrupt/generic.c \
interrupt/intstub.asm \
driver/serial.c \
driver/vgatext.c \
driver/pic.c \
driver/pit.c \
driver/pci.c \
debug/qemu_print.c \
debug/panic.c \
common/string.c \
common/conv.c \

KERNEL_OBJS:= ${KERNEL_SOURCES:.c=.o} 
KERNEL_OBJS:= ${KERNEL_OBJS:.asm=.o} 


LINK_LIST:=\
$(KERNEL_OBJS) \
$(LIBS) \

.PHONY: all clean install #install-headers install-kernel

all: smol-kernel

smol-kernel: $(KERNEL_OBJS) linker.ld
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(LINK_LIST)

%.o: %.c
	$(CC) -c $< -o $@ $(CPPFLAGS) $(CFLAGS) 

%.o: %.asm
	nasm -felf32 $< -o $@ 

clean:
	rm -f smol-kernel
	rm -f $(KERNEL_OBJS) 
