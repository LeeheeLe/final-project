#include "first_pass.h"
#include "label_table.h"
#include "mem_image.h"
#include "parsing.h"

#include <const_tables.h>
extern char *str_dup(const char *src);

#define MAX_OPERATION_LEN 3
#define IMMEDIATE_PARAM_INDICATOR '#'

#define REGISTER_INDICATOR 'r'
#define REGISTER_COUNT 8

#define RELATIVE_INDICATOR '&'

#define OPERAND_SEPARATOR ','

#define IGNORE_WHITESPACE(work_line)                                           \
  while (isspace(*work_line)) {                                                \
    work_line++;                                                               \
  }

enum op_type { DEST, SOURCE };

int is_data_instruction(inst instruction_type) {
  return instruction_type == DATA_INST || instruction_type == STRING_INST;
}
int is_linking_instruction(inst instruction_type) { /* is extern or entry*/
  return instruction_type == EXTERN_INST || instruction_type == ENTRY_INST;
}
void check_label_conflicts(enum errors *status, const table_head *table,
                           char *label_name, const int line_number) {
  if (find_label(label_name, *table) != NULL) {
    CONFLICTING_LABELS(line_number, label_name);
    *status = ERROR;
  }
}

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
    // todo: check for errors
  }
}

void write_str(memory data_image, int DC, char *str) {
  int i = 0;
  while (*str != '\0') {
    data_image[DC + i].character.value = *str;
    i++;
    str++;
  }
  data_image[DC + i].character.value = '\0';
}

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
    free(str);
  }
}

void handle_instruction(int DC, memory *data_image, enum errors *status,
                        table_head *table, char *work_line, char **label_name,
                        inst instruction_type, int line_number,
                        int label_flag) {
  if (is_data_instruction(instruction_type)) {
    if (label_flag) {
      add_label(table, *label_name, DC, DATA, DEFAULT);
    }
    handle_data_instruction(&DC, *data_image, status, work_line,
                            instruction_type, line_number);
  } else if (is_linking_instruction(instruction_type)) {
    if (label_flag) {
      LABELED_LINKING_WARNING(line_number);
    }
    if (instruction_type == EXTERN_INST) {
      *label_name = parse_extern(work_line, line_number, status);
      add_label(table, *label_name, DEFAULT_EXTERN_VALUE, EXTERNAL, EXTERN);
    }
  }
}

bool is_empty(struct operand_type type) {
  static struct operand_type empty_operand = {0, 0, 0, 0};
  return (type.ADDRESS == empty_operand.ADDRESS) &&
         (type.IMMEDIATE == empty_operand.IMMEDIATE) &&
         (type.REGISTER == empty_operand.REGISTER) &&
         (type.RELATIVE == empty_operand.RELATIVE);
}

bool handle_no_operand_operation(memory_word *temp, char **source_label,
                                 char **dest_label, char *line,
                                 operation_syntax syntax, int *value1) {
  if (is_whitespace(line)) {
    *source_label = NULL;
    *dest_label = NULL;
    temp->data.value = 0;
    temp->operation.opcode = syntax.opcode;
    *value1 = 1;
    return true;
  }
  // todo - error handling
  return false;
}
bool is_register(char *operand) {
  if (*operand == REGISTER_INDICATOR) {
    const int register_number = *(operand + 1) - '0';
    if (register_number < 0 || register_number > REGISTER_COUNT) {
      // not a register, might be a label starting with r like "r8" or "right"
      return false;
    }
    if (is_whitespace(operand + 2)) {
      return true;
    }
    // might be a label like "r3d" which is valid
    return false;
  }
  return false;
}
int extract_operand(char *operand, memory_word temp[MAX_OPERATION_LEN],
                    char **operand_label, int operand_number, enum op_type type,
                    bool *relative) {
  char *endptr;
  *relative = false;
  if (*operand == IMMEDIATE_PARAM_INDICATOR) {
    *operand++;
    *operand_label = NULL;
    temp[operand_number].data.value = 0;
    temp[operand_number].operand.value = strtol(operand, &endptr, 10);
    temp[operand_number].operand.A = 1;
    if (type == DEST) {
      temp->operation.dest_type = 0;
      temp->operation.dest_reg = 0;
    } else if (type == SOURCE) {
      temp->operation.source_type = 0;
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
      temp->operation.dest_type = 3; // todo enum instead of value
    } else if (type == SOURCE) {
      temp->operation.source_reg = *(operand + 1) - '0';
      temp->operation.source_type = 3; // todo enum instead of value
    }
    return 0;
  }
  // label operand
  if (*operand == RELATIVE_INDICATOR) {
    *relative = true;
    if (type == DEST) {
      temp->operation.dest_type = 2;
      temp->operation.dest_reg = 0;
    } else if (type == SOURCE) {
      temp->operation.source_type = 2;
      temp->operation.source_reg = 0;
    }
    *operand++;
  }
  char *label = malloc(strlen(operand) + 1);
  int i;
  if (label == NULL) {
    MEM_ALOC_ERROR();
    free(label);
    return -1;
  }
  for (i = 0; true; operand++) {
    if (isspace(*operand) || *operand == '\0') {
      label[i] = '\0';
      break;
    }
    label[i] = *operand;
    i++;
  }
  *operand_label = label;
  if (*relative == false && type == DEST) {
    temp->operation.dest_type = 1;
    temp->operation.dest_reg = 0;
  } else if (*relative == false && type == SOURCE) {
    temp->operation.source_type = 1;
    temp->operation.source_reg = 0;
  }
  return 1;
}
int parse_operation(char **work_line, int line_number,
                    memory_word temp[MAX_OPERATION_LEN], enum errors *errors,
                    char **source_label, char **dest_label) {
  // this should parse the entire line, finding the operation and its operands
  int i;
  int word_count = 1;
  char *line = *work_line;
  char *copy = malloc(strlen(*work_line) + 1);
  if (copy == NULL) {
    MEM_ALOC_ERROR();
    return -1;
  }
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
                                    syntax, &word_count) == true)
      return word_count; // todo error handling
  } else if (is_empty(syntax.source_type)) {
    // one operand of a type from dest_type from found syntax, line should be
    // the operand with spaces
    IGNORE_WHITESPACE(line);
    if (*line == '\0') {
      MISSING_OPERAND(line_number);
      return -1;
    }
    bool relative = false;
    temp->operation.opcode = syntax.opcode;
    temp->operation.funct = syntax.funct;
    word_count += extract_operand(line, temp, dest_label, 1, DEST, &relative);
    if (dest_label != NULL) {
      /*todo add intern with relative and code/data label based on the
       * operation and check if the operand type is valid for the given syntax*/
    }
  } else if (!is_empty(syntax.destination_type) &&
             !is_empty(syntax.source_type)) {
    temp->operation.opcode = syntax.opcode;
    temp->operation.funct = syntax.funct;
    char *param1, *param2;
    bool relative1, relative2;
    param1 = malloc(strlen(*work_line) + 1);
    param2 = malloc(strlen(*work_line) + 1);
    if (param1 == NULL || param2 == NULL) {
      MEM_ALOC_ERROR();
      free(param1);
      free(param2);
      return -1;
    }
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
int handle_operation(char **work_line, enum errors *status, table_head *table,
                     int line_number, memory code_image, const int IC) {
  int i;
  static memory_word temp[MAX_OPERATION_LEN];
  for (i = 0; i < MAX_OPERATION_LEN; i++) {
    temp[i].data.value = 0;
  }
  char *source_label, *dest_label;
  int op_size = parse_operation(work_line, line_number, temp, status,
                                &source_label, &dest_label);
  if (source_label != NULL) {
    // todo add intern with IC+1
  }
  if (dest_label != NULL) {
    // todo add intern with IC+op_size-1
  }
  for (i = 0; i < op_size; i++) {
    code_image[IC + i] = temp[i];
  }
  return op_size;
}
void first_pass(const char *file_name) {
  int IC = 100, DC = 0;
  static memory data_image;
  static memory code_image;
  char *input_file;
  enum errors status = NORMAL;
  table_head *table = initialise_table();
  input_file = malloc(strlen(file_name) + strlen(ASSEMBLER_INPUT_EXT) + 1);
  if (input_file == NULL) {
    MEM_ALOC_ERROR();
    free(input_file);
    return;
  }
  strcpy(input_file, file_name);
  strcat(input_file, ASSEMBLER_INPUT_EXT);
  FILE *input = fopen(input_file, "r"); /*open input file in 'read' mode*/
  free(input_file);
  if (input == NULL) {
    FILE_OPEN_ERROR();
    return;
  }
  char *line, *work_line, *label_name;
  inst instruction_type;
  int line_number = 0;
  int label_flag;
  while ((work_line = line = getLine(input)) != NULL) {
    label_flag = 0;
    line_number++;
    if (is_whitespace(line) || is_comment(line)) {
      continue;
    }
    if (is_label(&work_line, &label_name)) {
      label_flag = 1;
      check_label_conflicts(&status, table, label_name, line_number);
    }
    if (is_instruction(&work_line, &instruction_type, line_number)) {
      if (instruction_type == INVALID_INST) {
        continue;
      }
      handle_instruction(DC, &data_image, &status, table, work_line,
                         &label_name, instruction_type, line_number,
                         label_flag);
      continue;
    }
    /*the line is an operation line, work_line is pointing to the start of the
     * operation or to a whitespace before it*/
    if (label_flag) {
      add_label(table, label_name, IC, CODE, DEFAULT);
    }
    IC += handle_operation(&work_line, &status, table, line_number, code_image,
                           IC);
    free(line);
  }
}
