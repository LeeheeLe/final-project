mcro INVALID_MACRO
    mov r1, r2
    jmp r7
mcroend

MAIN:  mov #10, r2
       INVALID_MACRO
       lea #-4, r3
       dec r9
       stop EXTRA_OPERAND
INVALID_LABEL: .data 10, , 20