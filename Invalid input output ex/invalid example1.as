; This example contains multiple errors
1_INVALID_LABEL: mov r1, #-10     ; Error: Label starts with a digit
mov r1          ; Error: Missing second operand
cmp #5, r9      ; Error: Register r9 does not exist
sub r2, STR     ; Error: STR not defined yet
str: .string abc ; Error: Missing quotes around string
clr r2, r3      ; Error: clr only accepts one operand
jmp r3          ; Error: jmp does not support register as an operand
.extern W@        ; Error: Invalid characters in label name
mov K, EXT_LABEL ; Error: EXT_LABEL used without declaration as external
.entry EXT_LABEL ; Error: EXT_LABEL cannot be both entry and extern
lea #1, r3       ; Error: Invalid addressing for source operand in lea
data: .data 3, , ; Error: Extra commas in data directive
END: jsr MAIN ; Error: MAIN not defined yet
stop EXTRA_OPERAND ; Error: stop does not accept operands