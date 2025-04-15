#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <preprocessor.h>

/*
 * File: first_pass.h
 * ------------------
 * This header declares the first pass function used in the assembler.
 * The first pass processes a preprocessed assembly file (.am), builds
 * the symbol table, and validates labels, instructions, and syntax.
 *
 * The first pass uses:
 *  - Preprocessed input from the macro processor
 *  - Macro table for macro validation
 *  - Error handling and reporting
 *
 * Dependencies:
 *  - errors.h              : For reporting syntax/logic errors
 *  - file_extensions.h     : For consistent file extension references
 *  - preprocessor.h        : For macro table structure
 *  - input.h               : For reading lines and tokens
 *  - Standard C libraries  : For string, I/O, and character functions
 */

/*
 * ASSEMBLER_INPUT_EXT is defined as the output of the preprocessor (.am)
 * This allows the assembler to work on preprocessed code without macros.
 */
#define ASSEMBLER_INPUT_EXT PREPROCESSOR_OUTPUT_EXT


#define MAX_OPERATION_LEN 3         /* Maximum length of an operation. */
#define IMMEDIATE_PARAM_INDICATOR '#'  /* Indicator for immediate parameter. */
#define REGISTER_INDICATOR 'r'         /* Indicator for register. */
#define REGISTER_COUNT 8              /* Number of registers. */
#define RELATIVE_INDICATOR '&'        /* Indicator for relative operand. */
#define OPERAND_SEPARATOR ','         /* Separator between operands. */

/*
 * Macro to skip whitespaces in a line.
 */
#define IGNORE_WHITESPACE(work_line)                                           \
  while (isspace(*work_line)) {                                                \
    work_line++;                                                               \
  }

/*
 * Enum representing the two operand types: source and destination.
 */
enum op_type { DEST, SOURCE };



void check_label_conflicts(enum errors *status, label_table_head *label_table,
                           char *label_name, const int line_number);

void write_str(memory data_image, int DC, char *str);

int extract_operand(char *operand, memory_word temp[MAX_OPERATION_LEN],
                    char **operand_label, int operand_number, enum op_type type,
                    int *relative);

void check_macro_conflicts(enum errors * errors, struct Macro_table * macro_table, char * intern_name,
                          int line_number);
/*
 * Function: first_pass
 * --------------------
 * Executes the first pass of the assembler on the given file.
 * This involves:
 *  - Parsing and validating each line
 *  - Collecting label definitions
 *  - Validating instructions and operands
 *  - Building the symbol table
 *  - Tracking memory addresses (IC/DC)
 *
 * Parameters:
 *  file_name   - The base name of the source file (without extension)
 *  macro_table - A pointer to the macro table, used for validating macro usage
 */
void first_pass(const char *file_name, Macro_table *macro_table);

#endif /* FIRST_PASS_H */