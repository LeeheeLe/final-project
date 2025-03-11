#include "second_pass.h"
#include "errors.h"
#include "input.h"
#include <stdio.h>
#include <string.h>

#include "parsing.h"

/*algorithm:
 * 1. read next line from file. if end of file go to 7
 * 2. if the first field in the row is a label, skip it
 * 3. if is it .data, .string or .extern, go to 1
 * 4. is it .entry? if not, go to 6
 * 5. add 'entry' in label table to the to the label (if label is not in the
 * table, error)
 * 6. complete the binary coding of the operands,  בהתאם לשיטות מיעון שבשימוש.
 *      for every operand that has a label, find it's value in the label table
 * (error if not in the table) if label is marked as external, add the address of
 * מילת המידע הרלוונטית לרשימת מילות מידע שמתייחסות לסמל חיצוני. go back to 1
 * 7. we have read the entire source code. if found errors stop here
 * 8. build output files*/

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
  FILE *input = fopen(input_file, "r"); /*open input file in 'read' mode*/
  free(input_file);
  if (input == NULL) {
    FILE_OPEN_ERROR();
    return NULL;
  }
  char *work_line, *line, *label_name;
  inst instruction_type;
  while ((work_line = line = getLine(input)) != NULL) { /*while not EOF*/
    line_number++;
    if (is_label(&work_line, &label_name)) {
      free(line);
      continue;
    }
    if (is_instruction(&work_line, &instruction_type, line_number)) {
      if (instruction_type == ENTRY_INST) {
        add_entry_to_label_table(line); /*if label not in the table, error in
                                           the implementation of func*/
        free(line);
      }
      free(line);
      continue;
    }
    /*stage 6: complete the binary coding of the operands*/
    find_value_in_label_table(line); /*return error if not in the table*/
  }
  if (is_entry_flag == 1) {
    add_extern_adress(line);
  }
}
  /* finished reading the file. go to 7: if found errors stop here */

  fclose(input);
  fclose(output);
}
build_output_files(input_file, output_file) {}

/* TODO: implement is_label() add_entry_to_label_table... */