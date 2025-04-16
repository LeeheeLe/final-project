#include "const_tables.h"
#include <ctype.h>
#include "errors.h"
#include "handle_text.h"
#include "memory_utility.h"
#include "parsing.h"
#include "first_pass.h"
#include "const_tables.h"
#include "utility.h"
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

/*
 * Function: parse_operation
 * Purpose: Parses an operation line to extract the operation type and its operands.
 *
 * Parameters:
 *   work_line - The line to parse.
 *   line_number - The current line number in the source file.
 *   temp - The temporary memory word array to store parsed data.
 *   errors - The current error status.
 *   source_label - Pointer to store the source operand label.
 *   dest_label - Pointer to store the destination operand label.
 *
 * Returns:
 *   - The number of words parsed.
 */
int parse_operation(char **work_line, int line_number,
                    memory_word temp[MAX_OPERATION_LEN], enum errors *errors,
                    char **source_label, char **dest_label) {
  /* this should parse the entire line, finding the operation and its operands*/
  int i;
  int relative;
  operation_syntax syntax;
  char *param1, *param2;
  int relative1, relative2;
  int word_count = 1;
  char *line = *work_line;
  char *copy = safe_alloc(strlen(*work_line) + 1);
  IGNORE_WHITESPACE(line);
  i = 0;
  while (1) {
    if (isspace(*line) || *line == '\0') {
      copy[i] = '\0';
      break;
    }
    copy[i] = *line;
    i++;
    line++;
  }
  syntax = find_operation(copy);
  if (is_empty(syntax.source_type) && is_empty(syntax.destination_type)) {
    if (handle_no_operand_operation(temp, source_label, dest_label, line,
                                    syntax, &word_count) == 1)
      return word_count; /* todo error handling*/
  } else if (is_empty(syntax.source_type)) {
    /*one operand of a type from dest_type from found syntax, line should be the operand with spaces*/
    IGNORE_WHITESPACE(line);
    if (*line == '\0') {
      MISSING_OPERAND(line_number);
      return -1;
    }
    relative = 0;
    temp->operation.opcode = syntax.opcode;
    temp->operation.funct = syntax.funct;
    word_count += extract_operand(line, temp, dest_label, 1, DEST, &relative);
  } else if (!is_empty(syntax.destination_type) &&
             !is_empty(syntax.source_type)) {
    temp->operation.opcode = syntax.opcode;
    temp->operation.funct = syntax.funct;
    param1 = safe_alloc(strlen(*work_line) + 1);
    param2 = safe_alloc(strlen(*work_line) + 1);
    IGNORE_WHITESPACE(line);
    for (i = 0; *line != '\0'; line++) {
      if (isspace(*line) || *line == OPERAND_SEPARATOR) {
        break;
      }
      param1[i] = *line;
      i++;
    }
    param1[i] = '\0';
    IGNORE_WHITESPACE(line);
    if (*line != OPERAND_SEPARATOR) {
      MISSING_COMMA(line_number);
      return -1;
    }
    line++;
    IGNORE_WHITESPACE(line);
    if (*line == '\0') {
      MISSING_OPERAND(line_number);
      return -1;
    }
    for (i = 0; *line != '\0'; line++) {
      if (isspace(*line)) {
        break;
      }
      param2[i] = *line;
      i++;
    }
    param2[i] = '\0';
    word_count +=
        extract_operand(param1, temp, source_label, 1, SOURCE, &relative1);
    word_count +=
        extract_operand(param2, temp, dest_label, 2, DEST, &relative2);
  }
  return word_count;
}