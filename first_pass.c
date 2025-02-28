#include "first_pass.h"

int is_whitespace(const char *line);
int is_comment(const char *line);

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
  enum instruction {
    DATA,
    STRING,
    ENTRY,
    EXTERN
  } instruction_type; // todo: extract to different file
  int line_number = 0;
  int label_flag = 0;
  while ((work_line = line = getLine(input)) != NULL) {
    line_number++;
    if (is_whitespace(line) || is_comment(line)) {
      continue;
    }
    if (is_label(&work_line, &label_name)) {
      label_flag = 1;
    }
    /*if (is_data_instruction(line, &instruction_type)) {
        if (label_flag) {
            insert_label();//todo: figure out what are the correct params here
        }
        //todo: find data type, encode it and increase DC accordingly
        continue;
    }
    if (is_linking_instruction(line, &instruction_type)) {
        if (instruction_type == EXTERN) {
            insert_label();// todo: figure out correct params here
        }
        continue;
    }
    if (label_flag) {
        insert_label();//todo: figure out correct params
    }*/

    free(line);
  }
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
  char *name = malloc(32 * sizeof(char)), *work_line = *line;
  while (isspace(*work_line)) {
    work_line++;
  }
  if (isalpha(*work_line)) {
    *name = *work_line;
    work_line++;
    name++;
    label_length++;
  } /*checks that first character is a letter*/
  for (; !isspace(*work_line); work_line++) {
    if (*work_line == LABEL_DEF_CHAR) {
      *name = '\0';
      *label_name = name - label_length;
      *line = ++work_line;
      return 1;
    }
    *name = *work_line;
    name++;
    label_length++;
  }
  return 0;
}