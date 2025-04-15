#ifndef CONST_TABLES_H
#define CONST_TABLES_H

/*
 * File: const_tables.h
 * --------------------
 * This header defines constants and structures for describing operation
 * syntax in the assembler. Each operation includes its name, opcode,
 * function code, and the allowed types of operands for source and
 * destination fields.
 *
 * Structures:
 *  - type: Represents the allowed addressing modes using bit fields.
 *  - operation_syntax: Holds full operation metadata for instruction parsing.
 *
 * Function:
 *  - find_operation: Retrieves the operation syntax by name.
 */

/*
 * Struct: type
 * Represents the allowed operand types for an operation's source or destination.
 * Each field is 1 bit wide and acts as a flag indicating permission.
 *
 * Fields:
 *  IMMEDIATE - Immediate addressing mode (e.g., #5)
 *  ADDRESS   - Direct addressing (e.g., LABEL)
 *  RELATIVE  - Relative addressing (e.g., %LABEL)
 *  REGISTER  - Register addressing (e.g., r3)
 */
typedef struct {
  unsigned int IMMEDIATE : 1;
  unsigned int ADDRESS : 1;
  unsigned int RELATIVE : 1;
  unsigned int REGISTER : 1;
} op_type;

/*
 * Struct: operation_syntax
 * Represents the syntax definition of an assembler instruction.
 *
 * Fields:
 *  name              - Instruction mnemonic (e.g., "mov", "add")
 *  opcode            - Binary operation code
 *  funct             - Additional function code for specific operations
 *  source_type       - Allowed types for the source operand
 *  destination_type  - Allowed types for the destination operand
 */
typedef struct operation {
  char *name;
  int opcode;
  int funct;
  op_type source_type;
  op_type destination_type;
} operation_syntax;

/*
 * Function: find_operation
 * Finds the operation syntax based on the given operation name.
 *
 * Parameters:
 *  name - The operation mnemonic to search for
 *
 * Returns:
 *  The matching operation_syntax structure with full metadata.
 */
operation_syntax find_operation(char *name);

#endif /* CONST_TABLES_H */