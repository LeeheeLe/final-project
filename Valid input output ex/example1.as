; Example showing all commands and macros usage
.extern EXT_LABEL
.entry ENTRY_LABEL

; Macro definitions
mcro INCREMENT
    inc r1
    inc r2
mcroend

mcro DECREMENT
    dec r3
    dec r4
mcroend

mcro RESET
    clr r1
    clr r5
mcroend

START: mov #10, r1
       cmp r1, #-5
       add r2, r3
       lea DATA, r4
       prn r1
       INCREMENT
       sub r3, r2
       DECREMENT
       RESET
       jmp &LOOP
       jsr EXT_LABEL
       bne &EXIT

LOOP:  red r6
       stop

ENTRY_LABEL: .data 7, 9, 11
DATA: .string "ASM"
FUNC: rts
EXIT: .data 4