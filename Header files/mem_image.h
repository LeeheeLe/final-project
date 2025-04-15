#ifndef MEM_IMAGE_H
#define MEM_IMAGE_H

/*
 * File: mem_image.h
 * -----------------
 * This header defines the in-memory representation of assembly instructions,
 * operands, and data used during the translation of source code into machine code.
 *
 * It includes structures and types for encoding instructions, operands,
 * characters, and data values, all packed into a uniform 24-bit memory word.
 *
 * Constants:
 *  - START_ADDRESS: Starting address of the memory in the assembler (usually 100).
 *  - MAX_MEM_SIZE : Maximum number of memory words (1024 entries).
 *
 * Types:
 *  - operand_type : Enum for addressing methods.
 *  - memory_word  : Union of all memory cell types.
 *  - memory       : The actual simulated memory array.
 */

#define START_ADDRESS 100        /* Initial memory address for code section */
#define MAX_MEM_SIZE 1<<10       /* Maximum memory size (1024 words) */

/*
 * Enum: operand_type
 * ------------------
 * Represents the addressing method of an operand.
 *
 * Values:
 *  IMMEDIATE - Immediate addressing (#number)
 *  DIRECT    - Direct addressing (label)
 *  RELATIVE  - Relative addressing (%label)
 *  REGISTER  - Register addressing (r0–r7)
 */
typedef enum {
  IMMEDIATE = 0,
  DIRECT = 1,
  RELATIVE = 2,
  REGISTER = 3
} operand_type;

/*
 * Struct: operation
 * -----------------
 * Represents a full instruction word in memory (24 bits).
 *
 * Fields:
 *  E           - External reference bit
 *  R           - Relocatable bit
 *  A           - Absolute bit
 *  funct       - Function field (5 bits)
 *  dest_reg    - Destination register (3 bits)
 *  dest_type   - Destination operand addressing type (2 bits)
 *  source_reg  - Source register (3 bits)
 *  source_type - Source operand addressing type (2 bits)
 *  opcode      - Instruction opcode (6 bits)
 */
typedef struct {
  unsigned int E : 1;
  unsigned int R : 1;
  unsigned int A : 1;
  unsigned int funct : 5;
  unsigned int dest_reg : 3;
  unsigned int dest_type : 2;
  unsigned int source_reg : 3;
  unsigned int source_type : 2;
  unsigned int opcode : 6;
} operation;

/*
 * Struct: operand
 * ---------------
 * Represents an operand word in memory (used for numeric/address values).
 *
 * Fields:
 *  E      - External bit
 *  R      - Relocatable bit
 *  A      - Absolute bit
 *  value  - Operand value (21 bits)
 */
typedef struct {
  unsigned int E : 1;
  unsigned int R : 1;
  unsigned int A : 1;
  unsigned int value : 21;
} operand;

/*
 * Struct: data
 * ------------
 * Represents a 24-bit data word (used for numeric .data entries).
 */
typedef struct {
  unsigned int value : 24;
} data;

/*
 * Struct: character
 * -----------------
 * Represents a character value, stored in 1 byte.
 * Used for storing .string instruction characters.
 */
typedef struct {
  char value;
} character;

/*
 * Union: memory_word
 * ------------------
 * A 24-bit word in memory can store either:
 *  - An instruction
 *  - A data value
 *  - A character
 *  - An operand
 */
typedef union {
  operation operation;
  data data;
  character character;
  operand operand;
} memory_word;

/*
 * Type: memory
 * ------------
 * Represents the full memory space for the assembler.
 * This array holds all encoded instructions, operands, and data words.
 */
typedef memory_word memory[MAX_MEM_SIZE];

#endif /* MEM_IMAGE_H */