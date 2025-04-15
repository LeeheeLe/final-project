#include "../Header files/parsing.h"
#include "../Header files/errors.h"
#include <Header Files/memory_utility.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Array of instructions supported by the assembler.
 * Each instruction has an associated type and name.
 */
instruction instructions[] = {
  {DATA_INST, "data"},
  {STRING_INST, "string"},
  {ENTRY_INST, "entry"},
  {EXTERN_INST, "extern"}
};

/*
 * Checks if the given line starts with a valid instruction.
 *
 * This function searches for an instruction (like "data", "string") at the start of
 * the line, skips over any whitespace, and sets the instruction type if found.
 * If the instruction is invalid, it calls an error handler.
 *
 * @param line The line of code to check for an instruction.
 * @param instruction_type Pointer to the instruction type to be set if a valid instruction is found.
 * @param line_number The line number of the current line for error handling.
 *
 * @return int Returns 1 if an instruction is found, 0 if no instruction is found.
 */
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

/*
 * Checks if a given line consists only of whitespace.
 *
 * This function iterates over the line, skipping any leading whitespace characters,
 * and checks if the remaining characters are all whitespace or if the string is empty.
 *
 * @param line The line to check.
 *
 * @return int Returns 1 if the line is empty or consists only of whitespace, otherwise 0.
 */
int is_whitespace(const char *line) {
  if (strlen(line) == 0) {
    return 1;
  }
  while (*line != '\0' && isspace(*line)) {
    line++;
  }
  return *line == '\0';
}

/*
 * Checks if the given line is a comment.
 *
 * A line is considered a comment if it starts with a semicolon (';').
 *
 * @param line The line to check.
 *
 * @return int Returns 1 if the line is a comment, otherwise 0.
 */
int is_comment(const char *line) {
  if (*line == ';') {
    return 1;
  }
  return 0;
}

int valid_label_char(char *work_line, int label_length) {
  return !isspace(*work_line) &&
         (isalnum(*work_line) || *work_line == LABEL_DEF_CHAR || *work_line == '_') &&
         label_length <= MAX_LABEL_LENGTH;
}
/*
 * Checks if a given line starts with a valid label.
 *
 * This function searches for a label at the beginning of the line,
 * validating that the label starts with a letter and only contains
 * alphanumeric characters or the label definition character. The label
 * name is returned through the `label_name` pointer.
 *
 * @param line The line to check for a label.
 * @param label_name Pointer to a string where the label name will be stored if
 * found.
 *
 * @return int Returns 1 if a valid label is found, 0 if no label is found.
 */
int is_label(char **line, char **label_name, int line_number) {
  int label_length = 0;
  char *work_name = safe_alloc(MAX_LABEL_LENGTH + 1), *work_line = *line;
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
  for (; valid_label_char(work_line, label_length); work_line++) {
    if (*work_line == LABEL_DEF_CHAR) {
      *work_name = '\0';
      *label_name = name;
      *line = ++work_line;
      if (label_length > MAX_LABEL_LENGTH) {
        LABEL_TOO_LONG(line_number);
        return 0;
      }
      return 1;
    }
    *work_name = *work_line;
    work_name++;
    label_length++;
  }
  free_ptr(name);
  return 0;
}

/*
 * Parses a string from the given line.
 *
 * This function looks for a string enclosed in quotation marks ('"'). It extracts
 * the string, skips over the quotation marks, and returns the extracted string.
 * If the string is malformed (like missing quotation marks or extra characters),
 * an error is generated.
 *
 * @param line The line from which to extract the string.
 * @param line_number The line number for error handling.
 * @param status Pointer to the status of the parsing process (either success or error).
 *
 * @return char* Returns the parsed string, or NULL if there was an error.
 */
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
    char *orig_str, *str = safe_alloc(strlen(work_line) * sizeof(char) + 1);
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
    free_ptr(orig_str);
    EXTRA_CHARS_STRING_ERROR(line_number);
    return NULL;
  }
  MISSING_STRING_INDICATOR(line_number);
  return NULL;
}

/*
 * Parses an extern label from the given line.
 *
 * This function extracts a label from the line, which consists of alphanumeric
 * characters, and ensures that it doesn't exceed a predefined length. It returns the
 * label name, or NULL if there is an error (like extra characters or missing label).
 *
 * @param line The line from which to extract the linking label.
 * @param line_number The line number for error handling.
 * @param status Pointer to the status of the parsing process (either success or error).
 *
 * @return char* Returns the linking label, or NULL if there was an error.
 */
char *parse_linking_instruction(char *line, int line_number, enum errors *status) {
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
    char *orig_str, *str = safe_alloc(strlen(work_line) * sizeof(char) + 1);
    orig_str = str;
    for (; *work_line != '\0' && !isspace(*work_line) && (isalnum(*work_line) || *work_line == '_');
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
    EXTRA_CHARS_LINKING_ERROR(line_number);
    return NULL;
  }
  return NULL;
}