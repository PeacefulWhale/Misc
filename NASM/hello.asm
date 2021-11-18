; compile with:
; nasm -f elf test.asm

SECTION .data
msg     db      'Hello, World!', 0Ah, 0h    ; 0Ah is \n, and 0h is \0

SECTION .text
global _start

_start: 

    mov     ebx, msg
    mov     eax, ebx
 
nextchar:               ; calculate the length of our string, I think there's a built in function for this tbh
    cmp     byte [eax], 0
    jz      finished
    inc     eax
    jmp     nextchar

finished:
    sub     eax, ebx    ; because eax was set to ebx, and we increased eax for each character, eax -= ebx = total length of string
    mov     edx, eax    ; edx = length of string
    mov     ecx, msg    ; ecx = message
    mov     ebx, 1      ; ebx = output file
    mov     eax, 4      ; eax + 4 = call write()
    int     80h         ; call kernel

    ; registers are cleared after calling kernel

    mov     ebx, 0      ; put 0 on ebx for exit code
    mov     eax, 1      ; use 1 for exit 
    int     80h         ; call kernel