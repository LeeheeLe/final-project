; Includes all commands and macros
.extern EXTERNAL_SYMBOL
.entry ENTRY_POINT

; Macro definitions
mcro INITIALIZE
    mov #20, r3
    mov #30, r4
mcroend

mcro JUMP_FLOW
    jmp &LABEL_ONE
    bne &LABEL_TWO
mcroend

mcro CONTROL_REGISTERS
    inc r3
    dec r4
mcroend

ENTRY_POINT: INITIALIZE
             lea STRING, r5
             cmp r3, r4
             CONTROL_REGISTERS
             prn r5
             JUMP_FLOW
LABEL_ONE:   clr r1
LABEL_TWO:   red r6
             stop
STRING:      .string "Assembly!"
DATA_SEG:    .data 11, 22, -33