bits 32
section .note.GNU-stack noalloc noexec nowrite progbits

section .data
    msg db "hello"

section .text
        ;multiboot 1 spec
        align 4
        dd 0x1BADB002              ;magic
        dd 0x00                  ;flags
        dd - (0x1BADB002 + (0x00))   ;checksum. m+f+c should be zero

        ;multiboot 2 spec
        ;align 4
        ;dd 0xE85250D6 ;magic
        ;dd 0x00 ;flags
        ;dd - (0xE85250D6 + 0x00)

global _start
global write_port
global read_port
global call_reset_vector
global disable_cursor
extern kmain
extern color_test

read_port:
	mov edx, [esp + 4]
			;al is the lower 8 bits of eax
	in al, dx	;dx is the lower 16 bits of edx
	ret

write_port:
	mov   edx, [esp + 4]
	mov   al, [esp + 4 + 4]
	out   dx, al
	ret

_start:
    cli
    mov esp, stack_space
    mov ebp, stack_space
    push ebx
    call kmain
    hlt

call_reset_vector:
    jmp word 0xffff:0000
    ret

section .bss
resb 8192; 8KB for stack
stack_space: