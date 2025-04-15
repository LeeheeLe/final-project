.extern BAD_LABEL
MAIN:  cmp r1, #10
       jsr FUNC, r3
       bne END
       add &100, MAIN
FUNC:  lea BAD_LABEL, r3
END:   prn r4
       rts
BAD_LABEL: .data 10
.entry BAD_LABEL