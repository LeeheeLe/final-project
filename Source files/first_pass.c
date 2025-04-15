/*
 * File: first_pass.c
 * Purpose: This file performs the first pass of an assembler program, parsing instructions and processing labels.
 * It generates the data and code images.
 */

#include <first_pass.h>
#include <preprocessor.h>
#include <const_tables.h>
#include <mem_image.h>
#include <parsing.h>
#include <second_pass.h>
#include <tables.h>
#include <memory_utility.h>
#include <ctype.h>

/*
 * Function: check_label_conflicts
 * Purpose: Checks if a label already exists in the label table and handles conflicts.
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
 * Function: write_str
 * Purpose: Writes a string to the data image.
 *
 * Parameters:
 *   data_image - The memory image for data storage.
 *   DC - The current data counter.
 *   str - The string to write.
 */
void write_str(memory data_image, int DC, char *str) {
  int i = 0;
  while (*str != '\0') {
    data_image[DC + i].data.value = 0; /* Zero out noise */
    data_image[DC + i].character.value = *str;
    i++;
    str++;
  }
  data_image[DC + i].data.value = 0;
}

/*
 * Function: extract_operand
 * Purpose: Extracts an operand from a given string and stores it in the temporary memory word.
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
  char *label;
  int i;
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
      /* todo throw error*/
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
  /* label operand */
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
  label = safe_alloc(strlen(operand) + 1);

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

void check_macro_conflicts(enum errors * errors, struct Macro_table * macro_table, char * intern_name,
                          int line_number) {
  if (is_saved_macro(intern_name, macro_table) != -1) {
    LABEL_MACRO_CONFLICT(line_number, intern_name);
    *errors = ERROR;
  }
}

/**
 * Function: first_pass
 * Purpose: Performs the first pass of the assembler by reading the input file and
 * processing each line.
 *
 * Parameters:
 *   file_name - The name of the input file to assemble.
 */
void first_pass(const char *file_name, struct Macro_table *macro_table) {
  char *line, *work_line, *intern_name;
  inst instruction_type;
  int label_flag;
  int IC = 100, DC = 0;
  char *input_file = add_extension(file_name, ASSEMBLER_INPUT_EXT);
  enum errors status = NORMAL;
  int line_number = 0;


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
    populate_labels(file_name, code_image, *label_table, *intern_table, IC);
    create_entry_file(file_name, *label_table, *entry_table, IC);
    create_ob_file(file_name, code_image, data_image, IC, DC);
  }
}