
#ifndef PREPROCCESOR_H
#define PREPROCCESOR_H
#define MACRO_START "mcro"
#define MACRO_END "mcroend"

#define PREPROCESSED_EXT ".as"
#define INPUT_EXT ".am"


#include "errors.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


struct line{
    char* line;
    struct line *next_line;
};
struct macro_table {
    char *macro_name;
    struct line *first_line;
    struct macro_table *next_macro;
};

void preprocess(const char *input_file, const char *output_file);
int mcrostart(const char *line);
int mcroend(const char *line, error_code *ecode, const int line_number);
int isreserved(char *word);
void insert_macro_name(const char *line,struct macro_table *curr_macro, error_code *ecode, int line_number);
int is_saved_macro(const char *line, struct macro_table *head, error_code *ecode);
void print_macro_contents_to_file(const int macro_idx,struct macro_table *head_macro, FILE *output);
void append_line_to_macro(char *line,struct macro_table *curr_macro);


#endif //PREPROCCESOR_H
