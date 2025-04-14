#include "../Header files/second_pass.h"
#include "../Header files/errors.h"
#include "../Header files/input.h"
#include "../Header files/parsing.h"
#include "../Header files/tables.h"
#include "Header files/mem_image.h"
#include <stdio.h>
#include <string.h>

char *add_extension(const char *filename, const char *extension) {
  char *new_filename;
  const int filename_length = (int) strlen(filename);
  const int extension_length = (int) strlen(extension);

  new_filename = (char *)malloc(filename_length + extension_length + 1);
  if (new_filename == NULL) {
    /*memory allocation failed */
    // todo: free all
    MEM_ALOC_ERROR();
    exit(1);
  }

  strcpy(new_filename, filename);
  strcat(new_filename, extension);
  return new_filename;
}

void create_ob_file(const char *file_name, const memory *code,
                    const memory *data, const int ICF, const int DCF) {
  char *file_ob_name = add_extension(file_name, OBJECT_FILE_EXTENTION);
  FILE *file_ob = fopen(file_ob_name, "w");
  free(file_ob_name);
  int i = 0, j = 0;

  if (file_ob == NULL) {
    free(file_ob_name);
    FILE_OPEN_ERROR();
    free_all_memory(); // TODO: implement
    exit(1);
  }

  fprintf(file_ob, "  %d %d\n", ICF, DCF); /* header */

  for (i = 0; i + START_ADDRESS < ICF; i++) { /* machine code */
    fprintf(file_ob, "%07d %06x\n", i + START_ADDRESS, code[i+START_ADDRESS]->data.value);
  }
  for (j = 0; j + ICF < DCF; j++) {
    fprintf(file_ob, "%07d %06x\n", j + ICF, data[j]->data.value);
  }

  fclose(file_ob);
}

void create_entry_file(char *file_name, label_table_head label_table, entry_table_head entry_table) {
  char *file_ent_name = add_extension(file_name, ENTRIES_FILE_EXTENTION);
  FILE *file_entry = fopen(file_ent_name, "w");
  free(file_ent_name);

  if (file_entry == NULL) {
    FILE_OPEN_ERROR();
    free_all_memory(); // TODO: implement
    exit(1);
  }

  entry_node *current;
  label_node *found_label;
  current = entry_table.root;

  while (current != NULL) {
    if ((found_label = find_label(current->name, label_table)) == NULL) {
      //todo: throw error entry name not defined in the code
    } else {
      if (found_label->value == DEFAULT_EXTERN_VALUE) {
        //todo: throw error extern cannot be intern too
      }
      fprintf(file_entry, "%s %7d", current->name, found_label->value);
    }
    current = current->next_entry;
  }
  fclose(file_entry);
}

void populate_labels(char *file_name, memory *code,label_table_head label_table, intern_table_head intern_table) {
  char *file_ext_name = add_extension(file_name, EXTERNALS_FILE_EXTENTION);
  FILE *file_extern = fopen(file_ext_name, "w");
  free(file_ext_name);

  if (file_extern == NULL) {
    FILE_OPEN_ERROR();
    free_all_memory(); // TODO: implement
    exit(1);
  }

  intern_node *current = intern_table.root;
  label_node *found_label;
  while (current != NULL) {
    if ((found_label = find_label(current->name, label_table)) == NULL) {
      //todo: throw error label used but not declared
    } else {
      if (current->type == immediate) {
        code[current->mem_place]->operand.value = found_label->value;
        if (found_label->linking_type == EXTERN) {
          code[current->mem_place]->operand.E = 1;
          code[current->mem_place]->operand.value = 0;
          fprintf(file_ext, "%s %7d", current->name, current->mem_place);
        } else {
          code[current->mem_place]->operand.R = 1;
        }
      } else if (current->type == relative) {
        if (found_label->linking_type == EXTERN) {
          //todo: throw error cannot use extern label as relative param
        } else {
          code[current->mem_place]->operand.A = 1;
          code[current->mem_place]->operand.value = found_label->value-current->mem_place;
        }
      }
    }
  }
  fclose(file_extern);

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
