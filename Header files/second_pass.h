#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "file_extensions.h"
#include "errors.h"
#include "input.h"
#include "parsing.h"
#include "tables.h"
#include "mem_image.h"
#include <stdio.h>
#include <string.h>
#define INPUT_EXT PREPROCESSOR_OUTPUT_EXT

void populate_labels(const char *file_name, memory *code,
                     label_table_head label_table,
                     intern_table_head intern_table, int ICF);
void create_entry_file(const char *file_name, label_table_head label_table,
                       entry_table_head entry_table, int ICF);
void create_ob_file(const char *file_name, const memory *code,
                    const memory *data, const int ICF, const int DCF);
#endif /*SECOND_PASS_H*/
