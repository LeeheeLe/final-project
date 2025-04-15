#ifndef HANDLE_TEXT_H
#define HANDLE_TEXT_H

#include <ctype.h>  /* For character classification functions like isdigit() */
#include <mem_image.h>
#include <tables.h>
#include <parsing.h>


/*
 * File: handle_text.h
 * -------------------
 * Purpose:
 *   Declares functions responsible for parsing and handling textual segments
 *   of an assembly source file during the first pass of the assembler.
 *   These functions parse instructions, data definitions, and numeric values,
 *   and convert them into memory representations.
 *
 * Responsibilities:
 *   - Validate and store numeric data into the data image
 *   - Process .data and .string directives
 *   - Analyze and extract operands for instructions
 *   - Handle instruction syntax and operand encoding
 *
 * Dependencies:
 *   - errors.h               : For error handling during parsing
 *   - tables (label, entry, intern) : For managing symbols and references
 *   - instruction.h          : For instruction enums and types
 *   - memory.h               : For data and code image structures
 */

/*
 * Function: handle_numbers
 * ------------------------
 * Parses a sequence of numbers from a .data directive and stores them
 * in the data image.
 *
 * Parameters:
 *   line        - The line containing the numeric data
 *   line_number - Line number (for error reporting)
 *   status      - Pointer to the error status
 *   data_image  - Memory array for storing data values
 *   DC          - Current value of the Data Counter
 *
 * Returns:
 *   Number of data words successfully written, or 0 if an error occurred
 */
int handle_numbers(char *line, int line_number, enum errors *status, memory data_image, int DC);

/*
 * Function: handle_data_instruction
 * ---------------------------------
 * Handles a .data or .string directive and stores corresponding values
 * into the data image.
 *
 * Parameters:
 *   DC              - Pointer to the data counter
 *   data_image      - Memory array representing the data segment
 *   status          - Pointer to the error status
 *   work_line       - Line of source code to process
 *   instruction_type - Either DATA or STRING directive type
 *   line_number     - Line number (for error reporting)
 */
void handle_data_instruction(int *DC, memory data_image, enum errors *status,
                             char *work_line, inst instruction_type, int line_number);

/*
 * Function: handle_instruction
 * ----------------------------
 * Parses a full line of code representing an instruction, and encodes it
 * into the data image or symbol tables accordingly.
 *
 * Parameters:
 *   DC             - Pointer to the data counter
 *   data_image     - Memory image where instruction operands are written
 *   status         - Pointer to the current error status
 *   label_table    - Pointer to the label symbol table
 *   entry_table    - Pointer to the .entry label table
 *   work_line      - Line of code to parse
 *   label_name     - Output: Pointer to label name, if defined
 *   instruction_type - The directive type to handle (.data, .string, etc.)
 *   line_number    - Current line number in the file
 *   label_flag     - Indicates if a label was declared on this line
 */
void handle_instruction(int *DC, memory *data_image, enum errors *status, label_table_head *label_table,
                        entry_table_head *entry_table, char *work_line, char **label_name, inst instruction_type,
                        int line_number, int label_flag);

/*
 * Function: handle_no_operand_operation
 * -------------------------------------
 * Parses and encodes operations with no operands (e.g., `rts`, `stop`).
 *
 * Parameters:
 *   temp         - Temporary memory word array to hold encoded instruction
 *   source_label - Output: pointer to source label (NULL for this type)
 *   dest_label   - Output: pointer to destination label (NULL for this type)
 *   line         - The line to process
 *   syntax       - Expected syntax structure for the operation
 *   value1       - Output: Value to write into instruction word (if any)
 *
 * Returns:
 *   1 if successfully parsed, 0 if syntax error occurred
 */
int handle_no_operand_operation(memory_word *temp, char **source_label, char **dest_label,
                                char *line, operation_syntax syntax, int *value1);

/*
 * Function: handle_operation
 * --------------------------
 * Parses a line containing an instruction (with or without operands),
 * validates operands, and encodes instruction into the code image.
 *
 * Parameters:
 *   work_line   - Pointer to the current line being parsed
 *   status      - Pointer to the error status
 *   table       - Pointer to intern label table (for references)
 *   line_number - Line number of the instruction in the source file
 *   code_image  - Memory array for storing encoded instruction words
 *   IC          - Instruction counter for current location in memory
 *
 * Returns:
 *   Number of words written to the code image (0 if error)
 */
int handle_operation(char **work_line, enum errors *status, intern_table_head *table, int line_number,
                     memory code_image, const int IC);

#endif /* HANDLE_TEXT_H */