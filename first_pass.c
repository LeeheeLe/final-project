#include "first_pass.h"
#include "parsing.h"

int is_data_instruction(inst instruction_type) {
  return instruction_type == DATA || instruction_type == STRING;
}
int is_linking_instruction(inst instruction_type) {
  return instruction_type == EXTERN || instruction_type == ENTRY;
}
void first_pass(const char *file_name) {
  int IC = 100, DC = 100;
  char *input_file;
  input_file = malloc(strlen(file_name) + strlen(ASSEMBLER_INPUT_EXT) + 1);
  if (input_file == NULL) {
    MEM_ALOC_ERROR();
    free(input_file);
    return;
  }
  input_file = strcpy(input_file, file_name);
  strcat(input_file, ASSEMBLER_INPUT_EXT);
  FILE *input = fopen(input_file, "r"); /*open input file in 'read' mode*/
  free(input_file);
  if (input == NULL) {
    FILE_OPEN_ERROR();
    return;
  }
  char *line, *work_line, *label_name;
  inst instruction_type;
  int line_number = 0;
  int label_flag = 0;
  while ((work_line = line = getLine(input)) != NULL) {
    line_number++;
    if (is_whitespace(line) || is_comment(line)) {
      continue;
    }
    if (is_label(&work_line, &label_name)) {
      label_flag = 1;/*todo: check existing labels*/
    }
    /*if (is_instruction(&work_line, &instruction_type, line_number)) {
      if (instruction_type == INVALID) {
        continue;
      }
      if (is_data_instruction(instruction_type)) {
        if (label_flag) {
          insert_label(); // todo: figure out what are the correct params here
        }
        // todo: find data type, encode it and increase DC accordingly
      } else if (is_linking_instruction(instruction_type)) {
        if (label_flag) {
          LABELED_LINKING_WARNING(line_number);
        }
        if (instruction_type == EXTERN) {
          insert_label(); // todo: figure out correct params here
        }
      }
      continue;
    }*/
    /*the line is an operation line, work_line is pointing to the start of the operation or to a whitespace before it*/
    /*if (label_flag) {
      insert_label(); // todo: figure out correct params
    }*/
    free(line);
  }
}

