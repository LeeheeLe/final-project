#include "preprocessor.h"
#include "input.h"
#include "parsing.h"
#include "first_pass.h"
#include "const_tables.h"
#include "memory_utility.h"
#include "file_extensions.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define IGNORE_WHITESPACE(work_line)                                           \
while (isspace(*work_line)) {                                                \
work_line++;                                                               \
}

char *reserved_names[28] = {
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
    "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne",
    "jsr", "red", "prn", "rts", "stop", ".data", ".string", ".entry", ".extern"
  };


/**
 * preprocess - Preprocesses a given file by expanding macros and handling
 * the preprocessor logic.
 * @file_name: The name of the file to preprocess.
 *
 * This function reads a file, identifies any macros, and expands them
 * according to the macro definitions. It writes the resulting content
 * to an output file. The output file will contain the code with macros
 * expanded.
 *
 * Returns: A pointer to the macro table (linked list of macros) or NULL if an error occurs.
 */
Macro_table *preprocess(const char *file_name) {
    enum errors ecode = NORMAL;
    struct Macro_table *curr_macro, *head_macro;
    char *input_file, *output_file;
    char *line;
    FILE *input, *output;

    int macro_idx = -1, line_number = -1;
    head_macro = safe_alloc(sizeof(struct Macro_table));
    curr_macro = head_macro;

    head_macro->first_line = NULL;
    head_macro->macro_name = NULL;
    head_macro->next_macro = NULL;

    input_file = add_extension(file_name, PREPROCESSOR_INPUT_EXT);
    output_file = add_extension(file_name, PREPROCESSOR_OUTPUT_EXT);

    input = fopen(input_file, "r"); /*open input file in 'read' mode*/
    output = fopen(output_file, "w"); /*open output file in 'write' mode*/

    if (output == NULL || input == NULL) {
        FILE_OPEN_ERROR();
        free_all_memory();
        return NULL;
    }

    line = NULL; /*pointer to a buffer for storing each line read from the file*/
    while ((line = getLine(input)) != NULL) {
        line_number++;
        if ((macro_idx = is_saved_macro(line, head_macro)) != -1) {
            /*find index of macro in line if line is a macro*/
            print_macro_contents_to_file(macro_idx, head_macro, output);
            continue;
        }
        if (mcro_start(line)) {
            if (curr_macro->macro_name != NULL) {
                curr_macro->next_macro = safe_alloc(sizeof(struct Macro_table));
                curr_macro = curr_macro->next_macro;
                curr_macro->first_line = NULL;
                curr_macro->macro_name = NULL;
                curr_macro->next_macro = NULL;
            }
            insert_macro_name(line, curr_macro, &ecode, line_number);
            free_ptr(line);
            while ((line = getLine(input)) != NULL) {
                line_number++;
                if (!mcro_end(line, &ecode, line_number)) {
                    append_line_to_macro(line, curr_macro);
                } else {
                    free_ptr(line);
                    break;
                }
            }
        } else {
            fputs(line, output);
            fputc('\n', output);
        }
        free_ptr(line);
    }
    fclose(input);
    fclose(output);
    return head_macro;
}

/**
 * append_line_to_macro - Adds a line to a macro's content.
 * @line: The line to be added to the macro.
 * @curr_macro: The current macro to append the line to.
 *
 * This function will append a line to the macro's list of lines.
 * If this is the first line of the macro, it will initialize the first line.
 */
void append_line_to_macro(char *line, struct Macro_table *curr_macro) {
    struct Macro_line *curr_line;
    if (curr_macro->first_line == NULL) {
        curr_macro->first_line = safe_alloc(sizeof(struct Macro_line));
        curr_macro->first_line->line = line;
        curr_macro->first_line->next_line = NULL;
        return;
    }
    curr_line = curr_macro->first_line;
    while (curr_line->next_line != NULL) {
        curr_line = curr_line->next_line;
    }
    curr_line->next_line = safe_alloc(sizeof(struct Macro_line));
    curr_line->next_line->line = line;
    curr_line->next_line->next_line = NULL;
}

/**
 * print_macro_contents_to_file - Writes the contents of a macro to the output file.
 * @macro_idx: The index of the macro to print.
 * @head_macro: The head of the macro table.
 * @output: The file to write the macro content to.
 *
 * This function writes all the lines of a macro to the provided output file.
 */
void print_macro_contents_to_file(const int macro_idx, struct Macro_table *head_macro, FILE *output) {
    int idx;
    struct Macro_table *curr_macro = head_macro;
    struct Macro_line *curr_line;

    for (idx = 0; idx < macro_idx; idx++) {
        curr_macro = curr_macro->next_macro;
    }
    curr_line = curr_macro->first_line;
    while (curr_line != NULL && curr_line->line != NULL) {
        fputs(curr_line->line, output);
        fputc('\n', output);
        curr_line = curr_line->next_line;
    }
}

/**
 * mcro_start - Checks if a line starts a macro definition.
 * @line: The line to check.
 *
 * This function checks whether the given line represents the start of a macro.
 * Returns: 1 if the line starts a macro, 0 otherwise.
 */
int mcro_start(const char *line) {
    int i, j;
    for (i = 0; isspace(*(line + i)) && i < strlen(line); i++) {
    }
    for (j = 0; *(line + i + j) == MACRO_START[j] && i < strlen(line) && j < strlen(MACRO_START); j++) {
    }
    if (j == strlen(MACRO_START) && isspace(*(line + i + j))) {
        return 1;
    }
    return 0;
}

/**
 * mcro_end - Checks if a line ends a macro definition.
 * @line: The line to check.
 * @ecode: The error code pointer for handling errors.
 * @line_number: The current line number being processed.
 *
 * This function checks whether the given line represents the end of a macro.
 * Returns: 1 if the line ends a macro, 0 otherwise.
 */
int mcro_end(const char *line, enum errors *ecode, const int line_number) {
    int i, j;
    for (i = 0; isspace(*(line + i)) && i < strlen(line); i++) {
    }
    for (j = 0; *(line + i + j) == MACRO_END[j] && i < strlen(line) && j < strlen(MACRO_END); j++) {
    }
    for (i = i + j; isspace(*(line + i)) && i < strlen(line); i++) {
    }
    if (j == strlen(MACRO_END)) {
        if (i == strlen(line)) {
            return 1;
        }
        *ecode = ERROR;
        EXTRA_CHARS_MACRO_ERROR(line_number);
        return 1;
    }
    return 0;
}

/**
 * is_reserved_name - Checks if a given macro name is reserved.
 * @mcro_name: The macro name to check.
 *
 * This function checks if the provided macro name is part of the list of
 * reserved names. Reserved names cannot be used as macro names.
 * Returns: 1 if the name is reserved, 0 otherwise.
 */
int is_reserved_name(char *mcro_name) {
    int i;
    for (i = 0; i < sizeof(reserved_names)/sizeof(reserved_names[0]); i++) {
        if (strcmp(reserved_names[i], mcro_name) == 0) {
            /*name is already reserved*/
            return 1;
        }
    }
    return 0; /*valid macro name*/
}

/**
 * insert_macro_name - Inserts a new macro name into the macro table.
 * @line: The line containing the macro name.
 * @curr_macro: The current macro being processed.
 * @ecode: The error code pointer for handling errors.
 * @line_number: The current line number being processed.
 *
 * This function extracts a macro name from the given line and inserts it
 * into the macro table. If the macro name is reserved, an error is thrown.
 */
void insert_macro_name(const char *line, struct Macro_table *curr_macro, enum errors *ecode, int line_number) {
    int i, j;
    char *macro_name = safe_alloc(strlen(line)+1);
    for (i = 0; isspace(*(line + i)) && i < strlen(line); i++) {
    } /*ignore whitespace*/
    for (j = 0; *(line + i + j) == MACRO_START[j] && i < strlen(line) && j < strlen(MACRO_START); j++) {
    }
    for (i = i + j; isspace(*(line + i)) && i < strlen(line); i++) {
    } /*ignore whitespace*/
    j = 0 ;
    while (isprint(*(line + i)) && !isspace(*(line + i)) && i < strlen(line)+1) {
        macro_name[j] = line[i];
        i++;
        j++;
    }
    macro_name[j] = '\0';
    if (is_reserved_name(macro_name)) {
        *ecode = MACRO_NAME_RESERVED(line_number);
    }

    for (; isspace(*(line + i)) && i < strlen(line); i++) {
    } /*ignore whitespace*/

    if (i == strlen(line)) {
        curr_macro->macro_name = macro_name;
        return;
    }
    *ecode = ERROR;
    EXTRA_CHARS_MACRO_ERROR(line_number);
}

/**
 * is_saved_macro - Checks if the given line matches any saved macro.
 * @line: The line to check.
 * @head: The head of the macro table.
 *
 * This function checks if the given line is a macro definition and matches
 * any previously defined macro in the macro table.
 * Returns: The index of the macro if found, -1 otherwise.
 */
int is_saved_macro(const char *line, struct Macro_table *head) {
    int k = 0;
    struct Macro_table *curr = head;
    char *orig ,*temp = safe_alloc(strlen(line)+1);
    orig = strcpy(temp, line);
    IGNORE_WHITESPACE(orig);
    temp = orig;
    while (isprint(*temp) && !isspace(*temp)) {
        temp++;
    }
    *temp = '\0';
    while (curr != NULL && curr->macro_name != NULL) {
        if (strcmp(curr->macro_name, orig) == 0) {
            free_ptr(orig);
            return k;
        }
        k++;
        if (curr->next_macro == NULL) {
            free_ptr(orig);
            return -1;
        }
        curr = curr->next_macro;
    }
    free_ptr(orig);
    return -1;
}