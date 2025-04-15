.extern EXT_REF
.entry MAIN_LABEL

; Macros
mcro DATA_INIT
    mov #5, r3
    mov #10, r4
mcroend

mcro JUMP_CONDITIONAL
    cmp r3, #-1
    bne &EXIT
mcroend

mcro FUNC_CALL
    jsr FUNC
    rts
mcroend

MAIN_LABEL: inc r3
            DATA_INIT
            lea STR, r5
            JUMP_CONDITIONAL
            prn r5
            FUNC_CALL
            stop
STR:        .string "Done!"
EXIT:       .data -15