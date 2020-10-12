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
mem/pfa.c \
mem/paging.c \
driver/serial.c \
debug/qemu_print.c \
debug/panic.c \
common/string.c \

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
