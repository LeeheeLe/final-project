#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

/*
 * File: preprocessor.h
 * --------------------
 * This header defines the functions and data structures related to the preprocessor.
 * It handles macro definitions, such as inserting macro names, checking reserved names,
 * and managing macro content. It also performs initial preprocessing of the input file.
 *
 * Constants:
 *  - MACRO_START: The identifier marking the start of a macro definition.
 *  - MACRO_END: The identifier marking the end of a macro definition.
 *
 * Functions:
 *  - preprocess: Handles the preprocessing of the input file.
 *  - mcro_start: Identifies the start of a macro definition.
 *  - mcro_end: Identifies the end of a macro definition and processes it.
 *  - is_reserved_name: Checks if a macro name is reserved.
 *  - insert_macro_name: Inserts a macro name into the macro table.
 *  - is_saved_macro: Checks if a macro has already been defined.
 *  - print_macro_contents_to_file: Writes macro contents to an output file.
 *  - append_line_to_macro: Appends a line to a macro's content.
 */

#define MACRO_START "mcro"      /* Identifier for macro start */
#define MACRO_END "mcroend"     /* Identifier for macro end */

/* Include the necessary headers */
#include "errors.h"
#include <stdio.h>

/**
 * reserved_names - A list of reserved names in the assembly language.
 *
 * These reserved names cannot be used as macro names, labels, or instructions.
 */
extern char *reserved_names[28];

typedef struct Macro_line{
  char* line;
  struct Macro_line *next_line;
} Macro_line;

typedef struct Macro_table{
  char *macro_name;
  Macro_line *first_line;
  struct Macro_table *next_macro;
} Macro_table;


/*
 * Function: preprocess
 * --------------------
 * Processes the input assembly file, scanning for macro definitions,
 * expanding macros, and preparing the file for further processing.
 *
 * Parameters:
 *  file_name - The name of the input file to preprocess.
 *
 * Returns:
 *  A pointer to the macro table containing all macros found in the file.
 */
Macro_table *preprocess(const char *file_name);

/*
 * Function: mcro_start
 * --------------------
 * Identifies the start of a macro definition, marked by the MACRO_START string.
 *
 * Parameters:
 *  line - The line being processed.
 *
 * Returns:
 *  1 if the line marks the start of a macro definition, 0 otherwise.
 */
int mcro_start(const char *line);

/*
 * Function: mcro_end
 * ------------------
 * Identifies the end of a macro definition and processes it. Marks the end of a macro.
 *
 * Parameters:
 *  line        - The line being processed.
 *  ecode       - A pointer to the error status variable.
 *  line_number - The line number for error reporting.
 *
 * Returns:
 *  1 if the line marks the end of a macro definition, 0 otherwise.
 */
int mcro_end(const char *line, enum errors *ecode, const int line_number);

/*
 * Function: is_reserved_name
 * --------------------------
 * Checks if the given macro name is a reserved keyword in the assembler.
 *
 * Parameters:
 *  mcro_name - The name of the macro being checked.
 *
 * Returns:
 *  1 if the macro name is reserved, 0 otherwise.
 */
int is_reserved_name(char *mcro_name);

/*
 * Function: insert_macro_name
 * ---------------------------
 * Inserts the macro name into the macro table. This function is called when a new
 * macro is encountered.
 *
 * Parameters:
 *  line        - The line containing the macro name.
 *  curr_macro  - A pointer to the current macro table.
 *  ecode       - A pointer to the error status variable.
 *  line_number - The line number of the macro definition.
 */
void insert_macro_name(const char *line, struct Macro_table *curr_macro, enum errors *ecode, int line_number);

/*
 * Function: is_saved_macro
 * ------------------------
 * Checks if the macro is already saved in the macro table.
 *
 * Parameters:
 *  line  - The line containing the macro name.
 *  head  - The head of the macro table.
 *
 * Returns:
 *  1 if the macro is already saved, 0 otherwise.
 */
int is_saved_macro(const char *line, struct Macro_table *head);

/*
 * Function: print_macro_contents_to_file
 * --------------------------------------
 * Writes the contents of a macro to an output file.
 *
 * Parameters:
 *  macro_idx     - The index of the macro in the macro table.
 *  head_macro    - The head of the macro table.
 *  output        - The output file to write the macro contents.
 */
void print_macro_contents_to_file(const int macro_idx, struct Macro_table *head_macro, FILE *output);

/*
 * Function: append_line_to_macro
 * ------------------------------
 * Appends a line of text to the current macro's content.
 *
 * Parameters:
 *  line        - The line of text to append.
 *  curr_macro  - The current macro being processed.
 */
void append_line_to_macro(char *line, struct Macro_table *curr_macro);

#endif /* PREPROCESSOR_H */