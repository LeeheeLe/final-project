#include "../Header files/second_pass.h"
#include "../Header files/errors.h"
#include "../Header files/input.h"
#include "../Header files/parsing.h"
#include "Header files/mem_image.h"
#include "Header files/tables.h"

#include <stdio.h>
#include <string.h>

char *add_extension(const char *file_name, const char *new_extension) {
  char *new_str;
  new_str = malloc(strlen(file_name) + strlen(new_extension) + 1);
  if (new_str == NULL) {
    MEM_ALOC_ERROR();
    return NULL;
  }
  strcpy(new_str, file_name);
  strcat(new_str, new_extension);
  return new_str;
}

void create_ob_file(char *file_ob_name, memory *code, memory *data, int ICF,
                    int DCF) {
  FILE *file_ob = fopen(file_ob_name, "w");
  int i = 0, j = 0;
  if (file_ob == NULL) {
    FILE_OPEN_ERROR();
    free_all_memory(); // TODO: implement
    exit(1);
  }

  fprintf(file_ob, "\t%d %d\t\n", ICF, DCF); /* header */

  for (i = START_ADDRESS; i < ICF; i++) { /* machine code */
    fprintf(file_ob, "%07d %06x\n", i, code[i]->data.value);
  }
  for (j = 0; j < DCF; j++) {
    fprintf(file_ob, "%07d %06x\n", j + ICF, data[j]->data.value);
  }
  fclose(file_ob);
}

void create_entry_file(const char *file_name, label_table_head *root) {
  char *entries_file_name = add_extension(file_name, EXTERNALS_FILE_EXTENTION);
  if (entries_file_name == NULL) {
    free(entries_file_name);
    MEM_ALOC_ERROR();
    exit(1);
  }
  FILE *file_entry = fopen(entries_file_name, "w");
  free(entries_file_name);
  if (file_entry == NULL) {
    FILE_OPEN_ERROR();
    exit(1);
  }
  //todo: go over entry list (not yet implemented) and search for every label in it for its value and print it
  fclose(file_entry);
}

void create_ext_file(const char *file_name, label_table_head *root) {
  char *externals_file_name = add_extension(file_name, EXTERNALS_FILE_EXTENTION);
  if (externals_file_name == NULL) {
    free(externals_file_name);
    MEM_ALOC_ERROR();
    exit(1);
  }
  FILE *file_externals = fopen(externals_file_name, "r");
  free(externals_file_name);
  if (file_externals == NULL) {
    FILE_OPEN_ERROR();
    exit(1);
  }
  /*todo: implement within the internals logic an extern list with all places that use an extern label
   * go over said table here and print it out*/
  fclose(file_externals);
}

/*algorithm:
 * 1. read next line from file. if end of file go to 7
 * 2. if the first field in the row is a label, skip it
 * 3. if is it .data, .string or .extern, go to 1
 * 4. is it .entry? if not, go to 6
 * 5. add 'entry' in label table to the to the label (if label is not in the
 * table, error)
 * 6. complete the binary coding of the operands,  בהתאם לשיטות מיעון שבשימוש.
 *      for every operand that has a label, find it's value in the label table
 * (error if not in the table) if label is marked as external, add the address
 * of מילת המידע הרלוונטית לרשימת מילות מידע שמתייחסות לסמל חיצוני. go back to 1
 * 7. we have read the entire source code. if found errors stop here
 * 8. build output files*/

/*
struct second_pass *second_pass(const char *file_name) {
  char *input_file;
  int line_number = 1;
  input_file = malloc(strlen(file_name) + strlen(INPUT_EXT) + 1);
  if (input_file == NULL) {
    MEM_ALOC_ERROR();
    return NULL;
  }
  input_file = strcpy(input_file, file_name);
  strcat(input_file, INPUT_EXT);
  FILE *input = fopen(input_file, "r"); /*open input file in 'read' mode#1#
  free(input_file);
  if (input == NULL) {
    FILE_OPEN_ERROR();
    return NULL;
  }
  char *work_line, *line, *label_name;
  inst instruction_type;
  while ((work_line = line = getLine(input)) != NULL) { /*while not EOF#1#
    line_number++;
    if (is_label(&work_line, &label_name)) {
      free(line);
      continue;
    }
    if (is_instruction(&work_line, &instruction_type, line_number)) {
      if (instruction_type == ENTRY_INST) {
        add_entry_to_label_table(line); /*if label not in the table, error in
                                           the implementation of func#1#
        free(line);
      }
      free(line);
      continue;
    }
    /*stage 6: complete the binary coding of the operands#1#
    find_value_in_label_table(line); /*return error if not in the table#1#
  }
  if (is_entry_flag == 1) {
    add_extern_adress(line);
  }
}
  /* finished reading the file. go to 7: if found errors stop here #1#

  fclose(input);
  fclose(output);
}
build_output_files(input_file, output_file) {}
*/
