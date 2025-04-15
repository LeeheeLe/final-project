#include "../Header files/preprocessor.h"
#include "../Header files/input.h"
#include <Header Files/memory_utility.h>
#include <Header files/file_extensions.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * reserved_names - A list of reserved names in the assembly language.
 *
 * These reserved names cannot be used as macro names, labels, or instructions.
 */
char *reserved_names[28] = {
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
    "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne",
    "jsr", "red", "prn", "rts", "stop", ".data", ".string", ".entry", ".extern"
};

struct Macro_line{
    char* line;
    struct Macro_line *next_line;
} Macro_line;

struct Macro_table {
    char *macro_name;
    struct Macro_line *first_line;
    struct Macro_table *next_macro;
}Macro_table;

/**
 * preprocess - Preprocesses a given file by expanding macros and handling
 * the preprocessor logic.
 * @file_name: The name of the file to preprocess.
 *
 * This function reads a file, identifies any macros, and expands them
 * according to the macro definitions. It writes the resulting content
 * to an output file.
 *
 * Returns: A pointer to the macro table.
 */
struct Macro_table *preprocess(const char *file_name) {
    int macro_idx, line_number = -1;
    enum errors ecode = NORMAL;
    struct Macro_table *curr_macro, *head_macro;
    head_macro = safe_alloc(sizeof(struct Macro_table));
    char *input_file, *output_file;
    curr_macro = head_macro;

    head_macro->first_line = NULL;
    head_macro->macro_name = NULL;
    head_macro->next_macro = NULL;

    input_file =add_extension(file_name, PREPROCESSOR_INPUT_EXT);
    output_file = add_extension(file_name, PREPROCESSOR_OUTPUT_EXT);

    FILE *input = fopen(input_file, "r"); /*open input file in 'read' mode*/
    FILE *output = fopen(output_file, "w"); /*open output file in 'write' mode*/

    if (output == NULL || input == NULL) {
        FILE_OPEN_ERROR();
        free_all_memory();
        exit(EXIT_FAILURE);
    }

    char *line = NULL; /*pointer to a buffer for storing each line read from the file*/
    while ((line = getLine(input)) != NULL) {
        line_number++;
        if ((macro_idx = is_saved_macro(line, head_macro, &ecode)) != -1) {
            /*find index of macro in line if line is a macro*/
            print_macro_contents_to_file(macro_idx, head_macro, output);
            continue;
        }
        if (mcro_start(line)) {
            insert_macro_name(line, curr_macro, &ecode, line_number);
            while ((line = getLine(input)) != NULL) {
                line_number++;
                if (!mcro_end(line, &ecode, line_number)) {
                    append_line_to_macro(line, curr_macro);
                } else {
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

/*
 * append_line_to_macro - Adds a line to a macro's content.
 * @line: The line to be added to the macro.
 * @curr_macro: The current macro to append the line to.
 *
 * This function will append a line to the macro's list of lines.
 */
void append_line_to_macro(char *line, struct Macro_table *curr_macro) {
    if (curr_macro->first_line == NULL) {
        curr_macro->first_line = safe_alloc(sizeof(struct Macro_line));
        curr_macro->first_line->line = line;
        curr_macro->first_line->next_line = NULL;
        return;
    }
    struct Macro_line *curr_line = curr_macro->first_line;
    while (curr_line->next_line != NULL) {
        curr_line = curr_line->next_line;
    }
    curr_line->next_line = safe_alloc(sizeof(struct Macro_line));
    curr_line->next_line->line = line;
    curr_line->next_line->next_line = NULL;
}

/*
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

/*
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

/*
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

/*
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

/*
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
    for (j = 0; isprint(*(line + i)) && !isspace(*(line + i)) && i < strlen(line); i++) {
        *(macro_name + j) = *(line + i);
        j++;
    }

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

/*
 * is_saved_macro - Checks if the given line matches any saved macro.
 * @line: The line to check.
 * @head: The head of the macro table.
 * @ecode: The error code pointer for handling errors.
 *
 * This function checks if the given line is a macro definition and matches
 * any previously defined macro in the macro table.
 * Returns: The index of the macro if found, -1 otherwise.
 */
int is_saved_macro(const char *line, struct Macro_table *head, enum errors *ecode) {
    int i, j, k = 0;
    struct Macro_table *curr = head;
    while (curr != NULL && curr->macro_name != NULL) {
        for (i = 0; isspace(*(line + i)) && i < strlen(line); i++) {
        } /*ignore whitespace*/
        for (j = 0; *(line + i + j) == curr->macro_name[j] && i < strlen(line) && j < strlen(curr->macro_name); j++) {
        }
        for (i = i + j; isspace(*(line + i)) && i < strlen(line); i++) {
        } /*ignore whitespace*/
        if (i == strlen(line)) {
            return k;
        }
        k++;
        if (curr->next_macro == NULL) {
            return -1;
        }
        curr = curr->next_macro;
    }
    return -1;
}