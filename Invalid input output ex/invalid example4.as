1START: mov r1, r2
       sub r3,
       jsr &EXTERNAL_LABEL
       cmp #5, r12
       prn STR
       .data , 7, 3
STR:   .string assembly
.extern EXTERNAL_LABEL
.extern EXTERNAL_LABEL