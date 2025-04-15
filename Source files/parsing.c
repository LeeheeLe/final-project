#include <parsing.h>
#include <errors.h>
#include <memory_utility.h>
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
 * Purpose:
 * This file implements functions that parse and validate various aspects of assembly code.
 * It includes functions to check if a line starts with a valid instruction, label, string,
 * or extern label, and handles errors related to invalid input.
 *
 * Key Functions:
 * - `is_instruction`: Checks if a line starts with a valid assembler instruction.
 * - `is_whitespace`: Checks if the line is empty or contains only whitespace.
 * - `is_comment`: Checks if the line is a comment.
 * - `valid_label_char`: Checks if a character is valid for labels.
 * - `is_label`: Checks if a line starts with a valid label.
 * - `parse_string`: Parses a string enclosed in quotation marks.
 * - `parse_linking_instruction`: Parses an extern label (linking label) from the line.
 */

/**
 * Title: Instruction Validation
 *
 * Purpose:
 * This function checks if a given line starts with a valid instruction like "data", "string", etc.
 * It skips any leading whitespace and matches the beginning of the line to one of the predefined
 * instruction names. If a valid instruction is found, it sets the instruction type.
 * If no valid instruction is found, an error is generated.
 *
 * @param line Pointer to the line being checked for a valid instruction.
 * @param instruction_type Pointer to the instruction type to set if an instruction is found.
 * @param line_number The current line number for error reporting.
 *
 * @return int Returns 1 if a valid instruction is found, 0 if no valid instruction is found.
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
    INVALID_INSTRUCTION(line_number);  /* Call error handler for invalid instruction.*/
    *instruction_type = INVALID_INST;
    return 1;
  }
  return 0;
}

/**
 * Title: Whitespace Check
 *
 * Purpose:
 * This function checks if a line consists only of whitespace characters or is empty.
 * It is useful for skipping over empty lines during parsing.
 *
 * @param line The line to check for whitespace.
 *
 * @return int Returns 1 if the line consists only of whitespace, 0 if it contains other characters.
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

/**
 * Title: Comment Check
 *
 * Purpose:
 * This function checks if the given line is a comment. A line is considered a comment
 * if it starts with the semicolon character (';').
 *
 * @param line The line to check for being a comment.
 *
 * @return int Returns 1 if the line is a comment, 0 if it is not.
 */
int is_comment(const char *line) {
  if (*line == ';') {
    return 1;
  }
  return 0;
}

/**
 * Title: Valid Label Character Check
 *
 * Purpose:
 * This helper function checks if a character is valid for use in a label. Labels can only
 * contain alphanumeric characters, the label definition character, or an underscore ('_').
 * It also ensures the label does not exceed the maximum allowed length.
 *
 * @param work_line The current character in the line to check.
 * @param label_length The current length of the label being built.
 *
 * @return int Returns 1 if the character is valid for use in a label, 0 otherwise.
 */
int valid_label_char(char *work_line, int label_length) {
  return !isspace(*work_line) &&
         (isalnum(*work_line) || *work_line == LABEL_DEF_CHAR || *work_line == '_') &&
         label_length <= MAX_LABEL_LENGTH;
}

/**
 * Title: Label Check
 *
 * Purpose:
 * This function checks if a given line starts with a valid label. A valid label starts with
 * a letter and consists of alphanumeric characters or the label definition character ('_').
 * If a valid label is found, it sets the label name and returns a success status.
 *
 * @param line Pointer to the line being checked for a label.
 * @param label_name Pointer to a string where the label name will be stored if found.
 * @param line_number The current line number for error reporting.
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
        LABEL_TOO_LONG(line_number);  /* Error handler for too long label.*/
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

/**
 * Title: String Parsing
 *
 * Purpose:
 * This function extracts a string enclosed in quotation marks from a line. It checks for
 * proper formatting (like matching quotation marks) and handles errors for malformed strings.
 *
 * @param line The line containing the string to extract.
 * @param line_number The line number for error reporting.
 * @param status Pointer to the status of the parsing process (either success or error).
 *
 * @return char* Returns the extracted string if successful, or NULL if there is an error.
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

/**
 * Title: Linking Instruction Parsing
 *
 * Purpose:
 * This function parses a linking label (extern label) from the line. The label is expected to
 * be alphanumeric, and the function checks for any extra characters or malformed input.
 *
 * @param line The line containing the extern label to extract.
 * @param line_number The line number for error handling.
 * @param status Pointer to the status of the parsing process (either success or error).
 *
 * @return char* Returns the linking label if found, or NULL if there is an error.
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