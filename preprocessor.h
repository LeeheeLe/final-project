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

struct line{
    char* line;
    struct line *next_line;
};
struct macro_table {
    char *macro_name;
    struct line *first_line;
    struct macro_table *next_macro;
};

struct macro_table *preprocess(const char *file_name);
int mcro_start(const char *line);
int mcro_end(const char *line, enum errors *ecode, const int line_number);
int is_reserved_name(char *mcro_name);
void insert_macro_name(const char *line,struct macro_table *curr_macro, enum errors *ecode, int line_number);
int is_saved_macro(const char *line, struct macro_table *head, enum errors *ecode);
void print_macro_contents_to_file(const int macro_idx,struct macro_table *head_macro, FILE *output);
void append_line_to_macro(char *line,struct macro_table *curr_macro);

#endif /*PREPROCESSOR_H*/
