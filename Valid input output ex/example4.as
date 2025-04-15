; Demonstrates all commands and macros usage
.extern EXT_VAR
.entry START_LABEL

; Macro definitions
mcro ZERO_OUT
    clr r3
    clr r4
mcroend

mcro FLIP_BITS
    not r1
    not r2
mcroend

mcro MATH_OPERATIONS
    add r3, r4
    sub r4, r3
mcroend

START_LABEL: mov #-12, r1
             ZERO_OUT
             lea DATA_LABEL, r2
             cmp r1, r2
             FLIP_BITS
             MATH_OPERATIONS
             prn r3
             red r4
             jsr SUBROUTINE
SUBROUTINE:  inc r5
             dec r6
             rts
DATA_LABEL:  .data 7, -5, 13
TEXT:        .string "Macros!"
END:         stop