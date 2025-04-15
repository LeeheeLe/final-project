#ifndef SECOND_PASS_H
#define SECOND_PASS_H

/*
 * File: second_pass.h
 * -------------------
 * This header defines the functions used during the second pass of the assembler.
 * The second pass involves resolving labels, creating the final object files, and
 * generating the entry file. It processes the intermediate code generated during
 * the first pass and performs actions like label population and file creation.
 *
 * Functions:
 *  - populate_labels: Resolves and populates labels in the code memory.
 *  - create_entry_file: Creates the entry file based on the entry table.
 *  - create_ob_file: Creates the object file containing the final code and data.
 */

#include "file_extensions.h"
#include "errors.h"
#include "input.h"
#include "parsing.h"
#include "tables.h"
#include "mem_image.h"
#include <stdio.h>
#include <string.h>

/*
 * Function: populate_labels
 * -------------------------
 * Resolves labels in the assembly code. This function processes the input file
 * to assign addresses to labels in the code and data sections.
 *
 * Parameters:
 *  file_name        - The name of the file being processed.
 *  code             - A pointer to the memory that will hold the assembled code.
 *  label_table      - The table of labels to resolve.
 *  intern_table     - The table of interned labels.
 *  ICF              - The initial code address for the code section.
 */
void populate_labels(const char *file_name, memory code,
                     label_table_head label_table,
                     intern_table_head intern_table, int ICF);

/*
 * Function: create_entry_file
 * ---------------------------
 * Creates the entry file that lists all entry points in the assembly program.
 * An entry point is a label that is used in the code to indicate a function or
 * a location to jump to.
 *
 * Parameters:
 *  file_name       - The name of the file being processed.
 *  label_table     - The table of labels to check for entry points.
 *  entry_table     - The table of entry points to be written to the entry file.
 *  ICF             - The initial code address (to ensure proper entry addresses).
 */
void create_entry_file(const char *file_name, label_table_head label_table,
                       entry_table_head entry_table, int ICF);

/*
 * Function: create_ob_file
 * ------------------------
 * Creates the object file containing the final assembled code and data.
 * The object file includes machine code for both instructions and data
 * and is used in the final stages of the assembly process.
 *
 * Parameters:
 *  file_name       - The name of the file being processed.
 *  code            - A pointer to the memory holding the assembled code.
 *  data            - A pointer to the memory holding the assembled data.
 *  ICF             - The initial code address.
 *  DCF             - The initial data address.
 */
void create_ob_file(const char *file_name, const memory code,
                    const memory data, const int ICF, const int DCF);

#endif /* SECOND_PASS_H */