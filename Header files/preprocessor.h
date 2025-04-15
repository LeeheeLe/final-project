#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H
#define MACRO_START "mcro"
#define MACRO_END "mcroend"


#include "file_extensions.h"
#include "errors.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define INPUT_EXT PREPROCESSOR_INPUT_EXT
#define OUTPUT_EXT PREPROCESSOR_OUTPUT_EXT

struct Macro_line{
    char* line;
    struct Macro_line *next_line;
} Macro_line;

struct Macro_table {
    char *macro_name;
    struct Macro_line *first_line;
    struct Macro_table *next_macro;
}Macro_table;

struct Macro_table *preprocess(const char *file_name);
int mcro_start(const char *line);
int mcro_end(const char *line, enum errors *ecode, const int line_number);
int is_reserved_name(char *mcro_name);
void insert_macro_name(const char *line,struct Macro_table *curr_macro, enum errors *ecode, int line_number);
int is_saved_macro(const char *line, struct Macro_table *head, enum errors *ecode);
void print_macro_contents_to_file(const int macro_idx,struct Macro_table *head_macro, FILE *output);
void append_line_to_macro(char *line,struct Macro_table *curr_macro);

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

#endif /*PREPROCESSOR_H*/
