#include "../Header files/parsing.h"
#include "../Header files/errors.h"
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
    work_line++;
    for (i=0; i < NUMBER_OF_INSTRUCTION_TYPES; i++) {
      if (strncmp(work_line, instructions[i].name, strlen(instructions[i].name)) == 0) {
        *line = work_line + strlen(instructions[i].name);
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
  } //todo: length check for the labels and defining the max length somewhere
  free(name);
  return 0;
}

char *parse_string(char *line, int line_number, enum errors *status) {
  char *work_line = line;
  while (isspace(*work_line)) {
    work_line++;
  }
  if (*work_line == '\0') {
    MISSING_INSTRUCTION_PARAM(line_number);
    *status = ERROR;
    return NULL;
  }
  if (*work_line == STR_INDICATOR) {
    char *orig_str, *str = malloc(strlen(work_line) * sizeof(char) + 1);
    orig_str = str;
    work_line++;
    while (*work_line != '\0') {
      if (*work_line != STR_INDICATOR) {
        *str++ = *work_line++;
      } else {
        work_line++;
        break;
      }
    }
    if (*work_line == '\0' || is_whitespace(work_line)) {
      *str = '\0';
      return orig_str;
    }
    free(orig_str);
    EXTRA_CHARS_STRING_ERROR(line_number);
    return NULL;
  }
  MISSING_STRING_INDICATOR(line_number);
  return NULL;
}
char *parse_extern(char *line, int line_number, enum errors *status) {
  char *work_line = line;
  while (isspace(*work_line)) {
    work_line++;
  }
  if (*work_line == '\0' || is_whitespace(work_line)) {
    MISSING_INSTRUCTION_PARAM(line_number);
    *status = ERROR;
    return NULL;
  }
  if (isalpha(*work_line)) {
    char *orig_str, *str = malloc(strlen(work_line) * sizeof(char) + 1);
    orig_str = str;
    for (; *work_line != '\0' && !isspace(*work_line) && isalnum(*work_line);
         work_line++) {
      *str++ = *work_line;
         }
    if (*work_line == '\0' || is_whitespace(work_line)) {
      *str = '\0';
      if (strlen(orig_str) > 31) {
        LABEL_TOO_LONG(line_number);
      }
      return orig_str;
    }
    EXTRA_CHARS_EXTERN_ERROR(line_number);
    return NULL;
  }
  return NULL;
}