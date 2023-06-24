; nasm -f elf64 syscall.asm
; ld -o syscall syscall.o  
; ./syscall
; echo $? ; to see return val

section .text
global _start

_start: 
    ; use exit syscall (1)
    mov eax, 1
    mov ebx, 65d ; 65 is the return value
    int 0x80 ; interrupt to kernel to execute syscall

; write a string to stdout
section data
