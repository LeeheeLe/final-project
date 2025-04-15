#ifndef PARSING_H
#define PARSING_H

#include "errors.h"

/*
 * File: parsing.h
 * ---------------
 * This header defines the functions and data structures used for parsing
 * the assembly source code. It includes the functionality to identify and
 * validate instructions, labels, and other key parts of the assembly syntax.
 *
 * Types:
 *  - inst: Represents different types of instructions.
 *  - instruction: Holds the instruction type and its name.
 *
 * Constants:
 *  - LABEL_DEF_CHAR: The character used to define labels (:).
 *  - INSTRUCTION_START_CHAR: The character indicating the start of an instruction (.).
 *  - STR_INDICATOR: The character used to mark string literals (").
 *  - DEFAULT_EXTERN_VALUE: Default value for external references.
 *  - MAX_LABEL_LENGTH: Maximum length allowed for label names.
 */

/* Enum: inst
 * -----------
 * Represents the different types of instructions in assembly.
 *
 * Values:
 *  DATA_INST    - Data instruction (e.g., .data)
 *  STRING_INST  - String instruction (e.g., .string)
 *  ENTRY_INST   - Entry instruction (e.g., .entry)
 *  EXTERN_INST  - External instruction (e.g., .extern)
 *  INVALID_INST - Invalid instruction type
 */
typedef enum {
  DATA_INST,
  STRING_INST,
  ENTRY_INST,
  EXTERN_INST,
  INVALID_INST
} inst;

/* Struct: instruction
 * --------------------
 * Represents an instruction type with its name.
 *
 * Fields:
 *  instruction - The type of instruction (e.g., .data, .string).
 *  name        - The actual name of the instruction.
 */
typedef struct {
  inst instruction;
  char *name;
} instruction;

/*
 * External variable: instructions
 * -------------------------------
 * Array of instructions used in the assembler. It maps instruction types
 * to their respective names and provides a way to lookup instructions.
 */
extern instruction instructions[];

/*
 * Constant: NUMBER_OF_INSTRUCTION_TYPES
 * -------------------------------------
 * The total number of different instruction types supported by the assembler.
 */
#define NUMBER_OF_INSTRUCTION_TYPES 4

/* Constant: LABEL_DEF_CHAR
 * -------------------------
 * The character that indicates the definition of a label. A label is
 * followed by a colon ":" (e.g., label_name:).
 */
#define LABEL_DEF_CHAR ':'

/* Constant: INSTRUCTION_START_CHAR
 * -------------------------------
 * The character that marks the start of an instruction. It is used to
 * distinguish instructions from other elements in the source code (e.g., .data).
 */
#define INSTRUCTION_START_CHAR '.'

/* Constant: STR_INDICATOR
 * -----------------------
 * The character used to mark the start and end of string literals in the source code.
 * This is typically used for .string instructions (e.g., .string "Hello").
 */
#define STR_INDICATOR '"'

/* Constant: DEFAULT_EXTERN_VALUE
 * ------------------------------
 * The default value assigned to external references (e.g., variables or labels).
 */
#define DEFAULT_EXTERN_VALUE 0

/* Constant: MAX_LABEL_LENGTH
 * ---------------------------
 * The maximum number of characters allowed in a label name. Labels longer than this
 * will be considered invalid.
 */
#define MAX_LABEL_LENGTH 32


/*
 * Function: parse_string
 * ----------------------
 * Parses a string instruction and validates its syntax.
 *
 * Parameters:
 *  line       - The line containing the string instruction.
 *  line_number - The line number for error reporting.
 *  status     - A pointer to the error status variable.
 *
 * Returns:
 *  A pointer to the parsed string, or NULL in case of an error.
 */
char *parse_string(char *line, int line_number, enum errors *status);

/*
 * Function: parse_linking_instruction
 * -----------------------------------
 * Parses a linking instruction (e.g., .extern, .entry) and validates its syntax.
 *
 * Parameters:
 *  line       - The line containing the linking instruction.
 *  line_number - The line number for error reporting.
 *  status     - A pointer to the error status variable.
 *
 * Returns:
 *  A pointer to the parsed instruction name, or NULL in case of an error.
 */
char *parse_linking_instruction(char *line, int line_number, enum errors *status);

int valid_label_char(char *work_line, int label_length);
char *parse_linking_instruction(char *line, int line_number, enum errors *status);
int parse_operation(char **work_line, int line_number,
                    memory_word temp[MAX_OPERATION_LEN], enum errors *errors,
                    char **source_label, char **dest_label);

#endif /* PARSING_H */