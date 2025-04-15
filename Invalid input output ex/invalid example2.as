; This example showcases a variety of error types
MOVING: mov &START, #-12    ; Error: Invalid combination of addressing modes
CYCLE: inc r9               ; Error: Invalid register 'r9'
       add STR, &1234       ; Error: Invalid label name (numeric only)
       bne START, END       ; Error: 'bne' accepts only one operand
SUBROUTINE: jsr START, r5   ; Error: Extra operand in 'jsr', which takes only one operand
STRING: .string Assembly    ; Error: Missing quotes around string
DATA_ARRAY: .data 5, 10,,   ; Error: Extra comma in '.data' directive
.extern r1                  ; Error: Register cannot be declared as external
.entry STRING, DATA_ARRAY   ; Error: Multiple operands for '.entry', which accepts only one