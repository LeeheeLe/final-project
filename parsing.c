#include "parsing.h"
#include "errors.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

instruction instructions[] = {
  {DATA_INST, "data"},
  {STRING_INST, "string"},
  {ENTRY_INST, "entry"},
  {EXTERN_INST, "extern"}
};

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
    *instruction_type = INVALID_INST;
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
  for (;!isspace(*work_line) && (isalnum(*work_line) || *work_line == LABEL_DEF_CHAR); work_line++) {
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