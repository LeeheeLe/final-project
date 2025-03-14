#include "first_pass.h"
#include "parsing.h"
#include "label_table.h"
#include "mem_image.h"

int is_data_instruction(inst instruction_type) {
  return instruction_type == DATA_INST || instruction_type == STRING_INST;
}
int is_linking_instruction(inst instruction_type) {
  return instruction_type == EXTERN_INST || instruction_type == ENTRY_INST;
}

int handle_numbers(char *line, int line_number, enum errors *status, memory data_image, int DC) {
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
  if (*work_line == '\0') {
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
}
void write_str(memory data_image, int DC, char *str) {
  int i = 0;
  while (*str != '\0') {
    data_image[DC+i].character.value = *str;
    i++;
    str++;
  }
  data_image[DC+i].character.value = '\0';
}
void handle_data_instruction(int *DC, memory data_image, enum errors status,
                             char *work_line, inst instruction_type,
                             int line_number) {
  if (instruction_type == DATA_INST) {
    int num_count =
        handle_numbers(work_line, line_number, &status, data_image, *DC);
    *DC += num_count;
  } else if (instruction_type == STRING_INST) {
    char *str = parse_string(work_line, line_number, &status);
    write_str(data_image, *DC, str);
    *DC += (int)strlen(str) + 1;
    free(str);
  }
}
void check_label_conflicts(enum errors *status, table_head *table,
                           char *label_name, int line_number) {
  if (find_label(label_name, *table) != NULL) {
    CONFLICTING_LABELS(line_number, label_name);
    *status = ERROR;
  }
}
void handle_instruction(int DC, memory *data_image, enum errors status,
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
      *label_name = parse_extern(work_line, line_number, &status);
      add_label(table, *label_name, DEFAULT_EXTERN_VALUE, EXTERNAL, EXTERN);
    }
  }
}
void first_pass(const char *file_name) {
  int IC = 100, DC = 0;
  static memory data_image;
  char *input_file;
  enum errors status = NORMAL;
  table_head *table = initialise_table();
  input_file = malloc(strlen(file_name) + strlen(ASSEMBLER_INPUT_EXT) + 1);
  if (input_file == NULL) {
    MEM_ALOC_ERROR();
    free(input_file);
    return;
  }
  input_file = strcpy(input_file, file_name);
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
      handle_instruction(DC, &data_image, status, table, work_line, &label_name,
                         instruction_type, line_number, label_flag);
      continue;
    }
    /*the line is an operation line, work_line is pointing to the start of the
     * operation or to a whitespace before it*/
    if (label_flag) {
      add_label(table, label_name, IC, CODE, DEFAULT);
    }
    // todo: calculate binary code of operation and encode it
    free(line);
  }
}
