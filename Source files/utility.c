#include <first_pass.h>
#include <preprocessor.h>
#include <const_tables.h>
#include <mem_image.h>
#include <parsing.h>
#include <second_pass.h>
#include <tables.h>
#include <memory_utility.h>
#include <ctype.h>

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

/*
 * Function: is_data_instruction
 * Purpose: Checks if the given instruction is a data-type instruction.
 *
 * Parameters:
 *   instruction_type - The type of instruction to check.
 *
 * Returns:
 *   - 1 if it is a data instruction (DATA_INST or STRING_INST).
 *   - 0 otherwise.
 */
int is_data_instruction(inst instruction_type) {
    return instruction_type == DATA_INST || instruction_type == STRING_INST;
}

/*
 * Function: is_linking_instruction
 * Purpose: Checks if the given instruction is a linking-type instruction (extern or entry).
 *
 * Parameters:
 *   instruction_type - The type of instruction to check.
 *
 * Returns:
 *   - 1 if it is a linking instruction (EXTERN_INST or ENTRY_INST).
 *   - 0 otherwise.
 */
int is_linking_instruction(inst instruction_type) {
    return instruction_type == EXTERN_INST || instruction_type == ENTRY_INST;
}

/*
 * Function: is_empty
 * Purpose: Checks if the given operand is empty (no valid data).
 *
 * Parameters:
 *   type - The operand type to check.
 *
 * Returns:
 *   - 1 if the operand is empty (all fields are 0).
 *   - 0 if the operand has non-zero values.
 */
int is_empty(op_type type) {
    static op_type empty_operand = {0, 0, 0, 0};
    return (type.ADDRESS == empty_operand.ADDRESS) &&
           (type.IMMEDIATE == empty_operand.IMMEDIATE) &&
           (type.REGISTER == empty_operand.REGISTER) &&
           (type.RELATIVE == empty_operand.RELATIVE);
}


/*
 * Function: is_register
 * Purpose: Checks if the given operand is a valid register.
 *
 * Parameters:
 *   operand - The operand string to check.
 *
 * Returns:
 *   - True if the operand represents a valid register (r0 to r7).
 *   - False otherwise.
 */
int is_register(char *operand) {
    if (*operand == REGISTER_INDICATOR) {
        const int register_number = *(operand + 1) - '0';
        if (register_number < 0 || register_number > REGISTER_COUNT) {
            /* not a register, might be a label starting with r like "r8" or "right" */
            return 0;
        }
        if (is_whitespace(operand + 2)) {
            return 1;
        }
        /* might be a label like "r3d" which is valid */
        return 0;
    }
    return 0;
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