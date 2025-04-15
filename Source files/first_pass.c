/*
 * File: first_pass.c
 * Purpose: This file performs the first pass of an assembler program, parsing instructions and processing labels.
 * It generates the data and code images.
 */

#include "../Header files/first_pass.h"
#include "../Header files/const_tables.h"
#include "../Header files/mem_image.h"
#include "../Header files/parsing.h"
#include "../Header files/second_pass.h"
#include "../Header files/tables.h"
#include "../Header Files/memory_utility.h"

/*todo: divide this huge file to mini files according to similar tasks*/

#define MAX_OPERATION_LEN 3         /* Maximum length of an operation.*/
#define IMMEDIATE_PARAM_INDICATOR '#'  /*Indicator for immediate parameter.*/
#define REGISTER_INDICATOR 'r'         /*Indicator for register.*/
#define REGISTER_COUNT 8              /*Number of registers.*/
#define RELATIVE_INDICATOR '&'        /*Indicator for relative operand.*/
#define OPERAND_SEPARATOR ','         /*Separator between operands.*/

/*
 * Macro to skip whitespaces in a line.
 */
#define IGNORE_WHITESPACE(work_line)                                           \
  while (isspace(*work_line)) {                                                \
    work_line++;                                                               \
  }

/*
 * Enum representing the two operand types: source and destination.
 */
enum op_type { DEST, SOURCE };

/*
 * Function: is_data_instruction
 * Checks if the given instruction is a data type instruction.
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
 * Checks if the given instruction is a linking type instruction (extern or entry).
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
 * Function: check_label_conflicts
 * Checks if a label already exists in the label table and handles conflicts.
 *
 * Parameters:
 *   status - Pointer to the current status of the assembler (for error handling).
 *   table - Pointer to the label table.
 *   label_name - The label name to check for conflicts.
 *   line_number - The line number where the conflict is found.
 *
 * Side Effects:
 *   - Sets the status to ERROR if a conflict is found.
 */
void check_label_conflicts(enum errors *status, label_table_head *label_table,
                           char *label_name, const int line_number) {
  if (find_label(label_name, *label_table) != NULL) {
    CONFLICTING_LABELS(line_number, label_name);
    *status = ERROR;
  }
}

/*
 * Function: handle_numbers
 * Parses numbers from a line, handling the parsing and storing them in the data image.
 *
 * Parameters:
 *   line - The line of text containing the numbers.
 *   line_number - The line number in the source file.
 *   status - Pointer to the assembler's current status (for error handling).
 *   data_image - The memory image for data storage.
 *   DC - The data counter, keeps track of the current data position.
 *
 * Returns:
 *   - The number of numbers parsed from the line.
 */
int handle_numbers(char *line, int line_number, enum errors *status,
                   memory data_image, int DC) {
  int i, num;
  char *end_ptr, *work_line = line;
  for (i = 1; 1; i++) {
    num = strtol(work_line, &end_ptr, 10);
    if (end_ptr == work_line) {
      MISSING_NUMBER_OR_EXTRA_COMMA(line_number);
    } else {
      work_line = end_ptr;
    }
    while (isspace(*work_line)) {
      work_line++;
    }
    if (*work_line == ',') {
      data_image[DC].data.value = num;
      DC++;
      work_line++;
    } else if (*work_line == '\0' || is_whitespace(work_line)) {
      data_image[DC].data.value = num;
      return i;
    } else {
      MISSING_COMMA(line_number);
      *status = ERROR;
      return 0;
    }
  }
}

/*
 * Function: write_str
 * Writes a string to the data image.
 *
 * Parameters:
 *   data_image - The memory image for data storage.
 *   DC - The current data counter.
 *   str - The string to write.
 */
void write_str(memory data_image, int DC, char *str) {
  int i = 0;
  while (*str != '\0') {
    data_image[DC + i].data.value = 0; /*zero out noise*/
    data_image[DC + i].character.value = *str;
    i++;
    str++;
  }
  data_image[DC + i].data.value = 0;
}

/*
 * Function: handle_data_instruction
 * Handles data-type instructions (DATA_INST, STRING_INST) by parsing and storing the data.
 *
 * Parameters:
 *   DC - Pointer to the current data counter.
 *   data_image - The memory image for data storage.
 *   status - Pointer to the assembler's current status (for error handling).
 *   work_line - The line of text containing the data instruction.
 *   instruction_type - The type of data instruction (DATA_INST or STRING_INST).
 *   line_number - The current line number.
 */
void handle_data_instruction(int *DC, memory data_image, enum errors *status,
                             char *work_line, inst instruction_type,
                             int line_number) {
  if (instruction_type == DATA_INST) {
    int num_count =
        handle_numbers(work_line, line_number, status, data_image, *DC);
    *DC += num_count;
  } else if (instruction_type == STRING_INST) {
    char *str = parse_string(work_line, line_number, status);
    write_str(data_image, *DC, str);
    *DC += (int)strlen(str) + 1;
  }
}

/*
 * Function: handle_instruction
 * Handles instructions (both data and linking) and updates the label table and memory images.
 *
 * Parameters:
 *   DC - The current data counter.
 *   data_image - The memory image for data storage.
 *   status - Pointer to the assembler's current status (for error handling).
 *   table - Pointer to the label table.
 *   work_line - The line containing the instruction to handle.
 *   label_name - Pointer to the label name (if present).
 *   instruction_type - The type of instruction.
 *   line_number - The current line number.
 *   label_flag - Indicates whether a label is present.
 */
void handle_instruction(int *DC, memory *data_image, enum errors *status,
                        label_table_head *label_table, entry_table_head *entry_table, char *work_line, char **label_name,
                        inst instruction_type, int line_number,
                        int label_flag) {
  if (is_data_instruction(instruction_type)) {
    if (label_flag) {
      add_label(label_table, *label_name, *DC, DATA, DEFAULT);
    }
    handle_data_instruction(DC, *data_image, status, work_line,
                            instruction_type, line_number);
  } else if (is_linking_instruction(instruction_type)) {
    if (label_flag) {
      LABELED_LINKING_WARNING(line_number);
    }
    if (instruction_type == EXTERN_INST) {
      *label_name = parse_linking_instruction(work_line, line_number, status);
      add_label(label_table, *label_name, DEFAULT_EXTERN_VALUE, EXTERNAL, EXTERN);
    }
    if (instruction_type == ENTRY_INST) {
      *label_name = parse_linking_instruction(work_line, line_number, status);
      add_new_entry(entry_table, *label_name);
    }
  }
}

/*
 * Function: is_empty
 * Checks if the given operand is empty (no valid data).
 *
 * Parameters:
 *   type - The operand type to check.
 *
 * Returns:
 *   - 1 if the operand is empty (all fields are 0).
 *   - 0 if the operand has non-zero values.
 */
int is_empty(struct operand_type type) {
  static struct operand_type empty_operand = {0, 0, 0, 0};
  return (type.ADDRESS == empty_operand.ADDRESS) &&
         (type.IMMEDIATE == empty_operand.IMMEDIATE) &&
         (type.REGISTER == empty_operand.REGISTER) &&
         (type.RELATIVE == empty_operand.RELATIVE);
}

/*
 * Function: handle_no_operand_operation
 * Handles operations that do not have operands, setting the appropriate values.
 *
 * Parameters:
 *   temp - The memory word to store the operation.
 *   source_label - The source operand label (if present).
 *   dest_label - The destination operand label (if present).
 *   line - The line of text containing the operation.
 *   syntax - The syntax structure for the operation.
 *   value1 - A pointer to an integer to store the value.
 *
 * Returns:
 *   - 1 if the operation was handled successfully.
 *   - 0 if there was an error.
 */
int handle_no_operand_operation(memory_word *temp, char **source_label,
                                 char **dest_label, char *line,
                                 operation_syntax syntax, int *value1) {
  if (is_whitespace(line)) {
    *source_label = NULL;
    *dest_label = NULL;
    temp->data.value = 0;
    temp->operation.opcode = syntax.opcode;
    temp->operation.A = 1;
    *value1 = 1;
    return 1;
  }
  // todo - error handling
  return 0;
}

/*
 * Function: is_register
 * Checks if the given operand is a valid register.
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
      /* not a register, might be a label starting with r like "r8" or "right"*/
      return 0;
    }
    if (is_whitespace(operand + 2)) {
      return 1;
    }
    /*might be a label like "r3d" which is valid*/
    return 0;
  }
  return 0;
}

/*
 * Function: extract_operand
 * Extracts an operand from a given string and stores it in the temporary memory word.
 *
 * Parameters:
 *   operand - The operand string to extract.
 *   temp - The temporary memory word array to store the extracted operand.
 *   operand_label - The label associated with the operand (if any).
 *   operand_number - The operand number (0 or 1).
 *   type - The operand type (source or destination).
 *   relative - A flag indicating if the operand is relative.
 *
 * Returns:
 *   - The number of operands extracted (1 or 0).
 */
int extract_operand(char *operand, memory_word temp[MAX_OPERATION_LEN],
                    char **operand_label, int operand_number, enum op_type type,
                    int *relative) {
  char *endptr;
  *relative = 0;
  if (*operand == IMMEDIATE_PARAM_INDICATOR) {
    operand++;
    *operand_label = NULL;
    temp[operand_number].data.value = 0;
    temp[operand_number].operand.value = strtol(operand, &endptr, 10);
    temp[operand_number].operand.A = 1;
    if (type == DEST) {
      temp->operation.dest_type = IMMEDIATE;
      temp->operation.dest_reg = 0;
    } else if (type == SOURCE) {
      temp->operation.source_type = IMMEDIATE;
      temp->operation.source_reg = 0;
    }
    if (endptr == operand || !is_whitespace(endptr)) {
      MISSING_OPERAND(*operand);
      // todo throw error
    }
    *operand_label = NULL;
    return 1;
  }
  if (is_register(operand)) {
    *operand_label = NULL;
    if (type == DEST) {
      temp->operation.dest_reg = *(operand + 1) - '0';
      temp->operation.dest_type = REGISTER;
    } else if (type == SOURCE) {
      temp->operation.source_reg = *(operand + 1) - '0';
      temp->operation.source_type = REGISTER;
    }
    return 0;
  }
  /* label operand*/
  if (*operand == RELATIVE_INDICATOR) {
    *relative = 1;
    if (type == DEST) {
      temp->operation.dest_type = RELATIVE;
      temp->operation.dest_reg = 0;
    } else if (type == SOURCE) {
      temp->operation.source_type = RELATIVE;
      temp->operation.source_reg = 0;
    }
    operand++;
  }
  char *label = safe_alloc(strlen(operand) + 1);
  int i;
  for (i = 0; 1; operand++) {
    if (isspace(*operand) || *operand == '\0') {
      label[i] = '\0';
      break;
    }
    label[i] = *operand;
    i++;
  }
  *operand_label = label;
  if (*relative == 0 && type == DEST) {
    temp->operation.dest_type = DIRECT;
    temp->operation.dest_reg = 0;
  } else if (*relative == 0 && type == SOURCE) {
    temp->operation.source_type = DIRECT;
    temp->operation.source_reg = 0;
  }
  return 1;
}

/*
 * Function: parse_operation
 * Parses an operation line to extract the operation type and its operands.
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
  int word_count = 1;
  char *line = *work_line;
  char *copy = safe_alloc(strlen(*work_line) + 1);
  IGNORE_WHITESPACE(line);
  for (i = 0; *line != '\0'; line++) {
    if (isspace(*line)) {
      copy[i] = '\0';
      break;
    }
    copy[i] = *line;
    i++;
  }
  operation_syntax syntax = find_operation(copy);
  if (is_empty(syntax.source_type) && is_empty(syntax.destination_type)) {
    if (handle_no_operand_operation(temp, source_label, dest_label, line,
                                    syntax, &word_count) == 1)
      return word_count; // todo error handling
  } else if (is_empty(syntax.source_type)) {
    /*one operand of a type from dest_type from found syntax, line should be the operand with spaces*/
    IGNORE_WHITESPACE(line);
    if (*line == '\0') {
      MISSING_OPERAND(line_number);
      return -1;
    }
    int relative = 0;
    temp->operation.opcode = syntax.opcode;
    temp->operation.funct = syntax.funct;
    word_count += extract_operand(line, temp, dest_label, 1, DEST, &relative);
  } else if (!is_empty(syntax.destination_type) &&
             !is_empty(syntax.source_type)) {
    temp->operation.opcode = syntax.opcode;
    temp->operation.funct = syntax.funct;
    char *param1, *param2;
    int relative1, relative2;
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

/**
 * Function: handle_operation
 * Handles the parsing and processing of an operation line in the assembly code.
 *
 * Parameters:
 *   work_line - The line containing the operation to parse.
 *   status - Pointer to the current status of the assembler.
 *   table - Pointer to the label table.
 *   line_number - The current line number.
 *   code_image - The memory image for the code.
 *   IC - The instruction counter, which tracks the current position in the code image.
 *
 * Returns:
 *   - The size of the operation parsed.
 */
int handle_operation(char **work_line, enum errors *status,
                     intern_table_head *table, int line_number,
                     memory code_image, const int IC) {
  int i;
  static memory_word temp[MAX_OPERATION_LEN];
  for (i = 0; i < MAX_OPERATION_LEN; i++) {
    temp[i].data.value = 0;
  }
  /*set A to 1 for the first word of the operation*/
  temp->operation.A = 1;
  char *source_label, *dest_label;
  int op_size = parse_operation(work_line, line_number, temp, status,
                                &source_label, &dest_label);
  if (source_label != NULL) {
    if (temp->operation.source_type == DIRECT) {
      add_new_intern(table, source_label, IC + 1, immediate);
    } else if (temp->operation.source_type == RELATIVE) {
      add_new_intern(table, source_label, IC + 1, relative);
    }
  }
  if (dest_label != NULL) {
    if (temp->operation.dest_type == DIRECT) {
      add_new_intern(table, dest_label, IC + op_size - 1, immediate);
    } else if (temp->operation.dest_type == RELATIVE) {
      add_new_intern(table, dest_label, IC + op_size - 1, relative);
    }
  }
  for (i = 0; i < op_size; i++) {
    code_image[IC + i] = temp[i];
  }
  return op_size;
}

void check_macro_conflicts(enum errors * errors, struct Macro_table * macro_table, char * intern_name,
                          int line_number) {
  if (is_saved_macro(intern_name, macro_table) != -1) {
    LABEL_MACRO_CONFLICT(intern_name, line_number);
    *errors = ERROR;
  }
}
/**
 * Function: first_pass
 * Performs the first pass of the assembler by reading the input file and
 * processing each line.
 *
 * Parameters:
 *   file_name - The name of the input file to assemble.
 */
void first_pass(const char *file_name, struct Macro_table *macro_table) {
  int IC = 100, DC = 0;
  char *input_file = add_extension(file_name, ASSEMBLER_INPUT_EXT);
  enum errors status = NORMAL;

  /*these need to get to the second pass*/
  memory data_image;
  memory code_image;
  label_table_head *label_table = initialise_label_table();
  entry_table_head *entry_table = initialise_entry_table();
  intern_table_head *intern_table = initialise_intern_table();
  /*up to here need to get to the second pass*/

  FILE *input = fopen(input_file, "r"); /*open input file in 'read' mode*/
  if (input == NULL) {
    FILE_OPEN_ERROR();
    free_all_memory();
    exit(EXIT_FAILURE);
  }
  char *line, *work_line, *intern_name;
  inst instruction_type;
  int line_number = 0;
  int label_flag;
  while ((work_line = line = getLine(input)) != NULL) {
    label_flag = 0;
    line_number++;
    if (is_whitespace(line) || is_comment(line)) {
      continue;
    }
    if (is_label(&work_line, &intern_name, line_number)) {
      label_flag = 1;
      check_label_conflicts(&status, label_table, intern_name, line_number);
      check_macro_conflicts(&status, macro_table, intern_name, line_number);
    }
    if (is_instruction(&work_line, &instruction_type, line_number)) {
      if (instruction_type == INVALID_INST) {
        continue;
      }
      handle_instruction(&DC, &data_image, &status, label_table, entry_table, work_line,
                         &intern_name, instruction_type, line_number,
                         label_flag);
      continue;
    }
    /*the line is an operation line, work_line is pointing to the start of the
     * operation or to a whitespace before it*/
    if (label_flag) {
      add_label(label_table, intern_name, IC, CODE, DEFAULT);
    }
    IC += handle_operation(&work_line, &status, intern_table, line_number, code_image,
                           IC);
    free_ptr(line);
  }
  if (status == NORMAL) {
    populate_labels(file_name, &code_image, *label_table, *intern_table, IC);
    create_entry_file(file_name, *label_table, *entry_table, IC);
    create_ob_file(file_name, &code_image, &data_image, IC, DC);
  }
}