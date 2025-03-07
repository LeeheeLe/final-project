#include "first_pass.h"
#include "parsing.h"
#include "label_table.h"

int is_data_instruction(inst instruction_type) {
  return instruction_type == DATA_INST || instruction_type == STRING_INST;
}
int is_linking_instruction(inst instruction_type) {
  return instruction_type == EXTERN_INST || instruction_type == ENTRY_INST;
}

int parse_data(char *line, int line_number, enum errors *status) {
  int i;
  char *work_line = malloc(strlen(line) + 1);
  strcpy(work_line, line);
  for (i=0; *work_line  != '\0'; i++) {
    while (isspace(*work_line)) {
      work_line++;
    }
    if (*work_line == '\0') {
      break;
    }
    //todo: parse the nums, write them to memory and check for errors
  }

  free(work_line);
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
    if (*work_line == '\0' || is_whitespace(*work_line)) {
      *str = '\0';
      return orig_str;
    }
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
    for (;*work_line != '\0' && !isspace(*work_line) && isalnum(*work_line); work_line++) {
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
void first_pass(const char *file_name) {
  int IC = 100, DC = 0;
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
      if (find_label(label_name, *table)!=NULL) {
        CONFLICTING_LABELS(line_number, label_name);
        status = ERROR;
      }
    }
    if (is_instruction(&work_line, &instruction_type, line_number)) {
      if (instruction_type == INVALID_INST) {
        continue;
      }
      if (is_data_instruction(instruction_type)) {
        if (label_flag) {
          add_label(table, label_name, DC, DATA, DEFAULT);
        }
        if (instruction_type == DATA_INST) {
          int num_count = parse_data(work_line, line_number, &status);
          // todo: parse data instruction, write to memory and increase DC accordingly
        } else if (instruction_type == STRING_INST) {
          char *str = parse_string(work_line, line_number, &status);
          DC += (int) strlen(str)+1;
          // todo: write str to memory
          free(str);
        }
      } else if (is_linking_instruction(instruction_type)) {
        if (label_flag) {
          LABELED_LINKING_WARNING(line_number);
        }
        if (instruction_type == EXTERN_INST) {
          label_name = parse_extern(work_line, line_number, &status);
          add_label(table, label_name, DEFAULT_EXTERN_VALUE, EXTERNAL, EXTERN);
        }
      }
      continue;
    }
    /*the line is an operation line, work_line is pointing to the start of the operation or to a whitespace before it*/
    if (label_flag) {
      add_label(table, label_name, IC, CODE, DEFAULT);
    }
    // todo: calculate binary code of operation and encode it
    free(line);
  }
}

