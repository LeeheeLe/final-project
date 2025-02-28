#include "first_pass.h"

int is_label(char **line, char **label_name);
int is_whitespace(const char *line);
int is_comment(const char *line);
int is_instruction(char **line, inst *instruction_type, int line_number);

int is_data_instruction(inst instruction_type) {
  return instruction_type == DATA || instruction_type == STRING;
}
int is_linking_instruction(inst instruction_type) {
  return instruction_type == EXTERN || instruction_type == ENTRY;
}
void first_pass(const char *file_name) {
  int IC = 100, DC = 100;
  char *input_file;
  input_file = malloc(strlen(file_name) + strlen(INPUT_EXT) + 1);
  if (input_file == NULL) {
    MEM_ALOC_ERROR();
    free(input_file);
    return;
  }
  input_file = strcpy(input_file, file_name);
  strcat(input_file, INPUT_EXT);
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
    if (is_instruction(&work_line, &instruction_type, line_number)) {
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
    }
    /*the line is an operation line, work_line is pointing to the start of the operation or to a whitespace before it*/
    if (label_flag) {
      insert_label(); // todo: figure out correct params
    }
    free(line);
  }
}

int is_instruction(char **line, inst *instruction_type, int line_number) {
  char *work_line = *line;
  int i;
  while (isspace(*work_line)) {
    work_line++;
  }
  if (*work_line == INSTRUCTION_START_CHAR) {
    for (i=0; i < NUMBER_OF_INSTRUCTION_TYPES; i++) {
      if (strncmp(work_line, instructions[i].name, strlen(instructions[i].name)) == 0) {
        *line = ++work_line;
        *instruction_type = instructions[i].instruction;
        return 1;
      }
    }
    INVALID_INSTRUCTION(line_number);
    *instruction_type = INVALID;
    return 1;
  }
  return 0;
}


int is_whitespace(const char *line) {
  if (strlen(line) == 0) {
    return 1;
  }
  while (*line != '\0' && isspace(*line)) {
    line++;
  }
  return *line == '\0';
}

int is_comment(const char *line) {
  if (*line == ';') {
    return 1;
  }
  return 0;
}

int is_label(char **line, char **label_name) {
  int label_length = 0;
  char *work_name = malloc(32 * sizeof(char)), *work_line = *line;
  char *name = work_name;
  while (isspace(*work_line)) {
    work_line++;
  }
  if (isalpha(*work_line)) {
    *work_name = *work_line;
    work_line++;
    work_name++;
    label_length++;
  } /*checks that first character is a letter*/
  for (; !isspace(*work_line) && isalnum(*work_line); work_line++) {
    if (*work_line == LABEL_DEF_CHAR) {
      *work_name = '\0';
      *label_name = name;
      *line = ++work_line;
      return 1;
    }
    *work_name = *work_line;
    work_name++;
    label_length++;
  }
  free(name);
  return 0;
}