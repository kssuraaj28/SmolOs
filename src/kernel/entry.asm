;This will be a multiboot compatible kernel :)
; The bootloader has loaded us into 32-bit protected mode on a x86
; machine. Interrupts are disabled. Paging is disabled. The processor
; state is as defined in the multiboot standard. The kernel has full
; control of the CPU. The kernel can only make use of hardware features
; and any code it provides as part of itself.
MBALIGN   equ     1<<0
MEMINFO   equ     1<<1
FLAGS     equ     MBALIGN | MEMINFO
MAGIC     equ     0x1BADB002
CHECKSUM  equ     -(MAGIC + FLAGS)

KERNEL_VMA  equ 0xC0100000
KERNEL_PMA  equ 0x100000
VM_BASE     equ KERNEL_VMA - KERNEL_PMA
PDE_INDEX   equ (VM_BASE >> 22)


section .multiboot
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM

section .bss
align 16
stack_bottom:
  resb 16384 ;16KB stack
stack_top:

section .data
global _is_identity_map
_is_identity_map: db 1
align 4096
global _init_page_dir
_init_page_dir:
    dd 0x00000083
    times(PDE_INDEX - 1) dd 0
    dd 0x00000083
    times(1024 - PDE_INDEX - 1) dd 0 

section .text

global _start
_start:
    ; update page directory address, since eax and ebx is in use, have to use ecx or other register
    mov ecx, (_init_page_dir - VM_BASE)
    mov cr3, ecx

    ; Enable 4mb pages
    mov ecx, cr4
    or ecx, 0x10 ;PSE enable
    mov cr4, ecx

    ; Set PG bit, enable paging
    mov ecx, cr0
    or ecx, 0x80000000
    mov cr0, ecx

    lea ecx, [higher_half]
    jmp ecx

    higher_half:
    mov esp, stack_top

    push eax
    push ebx
extern kmain
    call kmain

    cli
    jmp $

