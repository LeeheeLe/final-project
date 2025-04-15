.extern EXTERNAL_VAR
.entry ENTRY_POINT

; Macros
mcro INIT_REGISTERS
    clr r1
    clr r2
mcroend

mcro COUNTER_CONTROL
    inc r1
    dec r2
mcroend

mcro ARITHMETIC_OPS
    add r1, r3
    sub r3, r2
mcroend

START: mov #-7, r1
       INIT_REGISTERS
       lea DATA_ARRAY, r4
       cmp r4, r6
       ARITHMETIC_OPS
       prn r4
       COUNTER_CONTROL
       red r7
       jmp &LOOP
LOOP:  jsr SUBROUTINE
       bne &END
SUBROUTINE: not r3
            rts
END:    stop
ENTRY_POINT: .data 10, 20, -5
DATA_ARRAY: .string "Assembly"