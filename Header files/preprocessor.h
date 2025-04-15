#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#define MACRO_START "mcro"
#define MACRO_END "mcroend"

#include "errors.h"
#include <stdio.h>

struct Macro_table *preprocess(const char *file_name);
int mcro_start(const char *line);
int mcro_end(const char *line, enum errors *ecode, const int line_number);
int is_reserved_name(char *mcro_name);
void insert_macro_name(const char *line,struct Macro_table *curr_macro, enum errors *ecode, int line_number);
int is_saved_macro(const char *line, struct Macro_table *head);
void print_macro_contents_to_file(const int macro_idx,struct Macro_table *head_macro, FILE *output);
void append_line_to_macro(char *line,struct Macro_table *curr_macro);

#endif /* PREPROCESSOR_H */