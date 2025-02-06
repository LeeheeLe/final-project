//
// Created by itay on 1/31/25.
//

#ifndef PREPROCCESOR_H
#define PREPROCCESOR_H
#define MACRO_START "mcro"
#define MACRO_END "mcroend"

#include "errors.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


void preprocces(const char *input_file, const char *output_file);
int mcrostart(const char *line);
int mcroend(const char *line, error_code *ecode);
int isreserved(char *word);
void insert_macro_name(const char *line,struct macro_table *curr_macro, error_code *ecode);
int is_saved_macro(const char *line, const struct macro_table *head, error_code *ecode);


#endif //PREPROCCESOR_H
