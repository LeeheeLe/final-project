#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <mem_image.h>
#include <preprocessor.h>
#include <tables.h>

/**
 * File: first_pass.h
 * ------------------
 * Purpose:
 *   Declares the interface for the first pass of a two-pass assembler.
 *   The first pass processes a preprocessed assembly file (.am), identifies
 *   and validates labels and instructions, builds the symbol table, and
 *   prepares memory images (IC and DC) for the second pass.
 *
 * Responsibilities:
 *   - Parse and validate each line of the input assembly source
 *   - Collect label definitions and handle label conflicts
 *   - Validate macro usage and instruction syntax
 *   - Extract and encode operands
 *   - Track instruction and data memory counters (IC and DC)
 *   - Report syntax and logic errors
 *
 * Dependencies:
 *   - errors.h              : For error enumeration and reporting
 *   - file_extensions.h     : For handling standard file extensions
 *   - preprocessor.h        : For access to the macro table structure
 *   - input.h               : For reading and parsing input lines
 *   - Standard C libraries  : For string manipulation and character checks
 */

/* Define the input file extension for the assembler (.am) */
#define ASSEMBLER_INPUT_EXT PREPROCESSOR_OUTPUT_EXT

/* Constants for syntax recognition and validation */
#define MAX_OPERATION_LEN 3             /* Maximum number of words for an operation */
#define IMMEDIATE_PARAM_INDICATOR '#'   /* Prefix for immediate values */
#define REGISTER_INDICATOR 'r'          /* Prefix for register operands */
#define REGISTER_COUNT 8                /* Total number of valid registers */
#define RELATIVE_INDICATOR '&'          /* Prefix for relative address operands */
#define OPERAND_SEPARATOR ','           /* Character used to separate operands */

/**
 * Macro: IGNORE_WHITESPACE
 * -------------------------
 * Advances a pointer past leading whitespace characters.
 *
 * Parameters:
 *   work_line - A pointer to a character array (string)
 */
#define IGNORE_WHITESPACE(work_line)                                           \
  while (isspace(*work_line)) {                                                \
    work_line++;                                                               \
  }

/**
 * Enum: op_type
 * -------------
 * Represents the type of operand in an instruction:
 *   DEST   - Destination operand
 *   SOURCE - Source operand
 */
enum op_type { DEST, SOURCE };

/**
 * Function: check_label_conflicts
 * -------------------------------
 * Checks whether a given label name conflicts with existing labels
 * or reserved keywords, and updates the error status if necessary.
 *
 * Parameters:
 *   status      - Pointer to the current error status enum
 *   label_table - Pointer to the symbol table of labels
 *   label_name  - The label name to check
 *   line_number - The line number of the current input line (for error reporting)
 */
void check_label_conflicts(enum errors *status, label_table_head *label_table,
                           char *label_name, const int line_number);

/**
 * Function: write_str
 * -------------------
 * Writes a string (null-terminated) to the data image, starting at DC.
 * Each character is stored as an individual word.
 *
 * Parameters:
 *   data_image - The memory array representing the data image
 *   DC         - The current data counter
 *   str        - The string to write into memory
 */
void write_str(memory data_image, int DC, char *str);

/**
 * Function: extract_operand
 * -------------------------
 * Parses and encodes an operand from a string into memory words.
 *
 * Parameters:
 *   operand        - The raw operand string
 *   temp           - Temporary memory words to hold the operand encoding
 *   operand_label  - Output: pointer to extracted label (if applicable)
 *   operand_number - Position of the operand (1st or 2nd)
 *   type           - Operand type (SOURCE or DEST)
 *   relative       - Output: flag indicating if the operand is relative
 *
 * Returns:
 *   The number of words used to encode the operand (0 if error)
 */
int extract_operand(char *operand, memory_word temp[MAX_OPERATION_LEN],
                    char **operand_label, int operand_number, enum op_type type,
                    int *relative);

/**
 * Function: check_macro_conflicts
 * -------------------------------
 * Checks for conflicts between a new label and existing macro definitions.
 * If a conflict is found, the appropriate error is raised.
 *
 * Parameters:
 *   errors       - Pointer to the current error status
 *   macro_table  - Pointer to the macro table structure
 *   intern_name  - Name of the label being validated
 *   line_number  - The line number for reporting
 */
void check_macro_conflicts(enum errors * errors, struct Macro_table * macro_table, char * intern_name,
                           int line_number);

/**
 * Function: first_pass
 * --------------------
 * Executes the first pass of the assembler on the specified input file.
 * This function opens the .am file, reads it line by line, validates syntax,
 * builds the symbol table, handles macros, and constructs memory images.
 *
 * Parameters:
 *   file_name   - The base name of the source file (without extension)
 *   macro_table - A pointer to the macro table, used for validating macro usage
 *
 * Output:
 *   - Populates symbol table
 *   - Fills data image
 *   - Tracks IC/DC
 *   - Reports errors to the user
 */
void first_pass(const char *file_name, Macro_table *macro_table);

#endif /* FIRST_PASS_H */