; Declare constants for the multiboot header.
%define ALIGN    (1 << 0)         ; Align loaded modules on page boundaries
%define MEMINFO  (1 << 1)         ; Provide memory map
%define FLAGS    (ALIGN | MEMINFO) ; This is the Multiboot 'flag' field
%define MAGIC    0x1BADB002       ; 'Magic number' lets bootloader find the header
%define CHECKSUM -(MAGIC + FLAGS) ; Checksum of above, to prove we are multiboot

; Declare multiboot header with magic values.
section .multiboot
    align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .bss
    align 16
stack_bottom:
    resb 16384 
stack_top:

; Kernel entry point.
section .text
global _start
_start:
    ; Set up the stack pointer
    mov esp, stack_top
	extern kernel_main
    call kernel_main
    cli
halt:      
	hlt
    jmp halt
