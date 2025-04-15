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
 * Function: handle_numbers
 * Purpose: Parses numbers from a line, handling the parsing and storing them in the data image.
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
 * Function: handle_data_instruction
 * Purpose: Handles data-type instructions (DATA_INST, STRING_INST) by parsing and storing the data.
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
 * Purpose: Handles instructions (both data and linking) and updates the label table and memory images.
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
 * Function: handle_no_operand_operation
 * Purpose: Handles operations that do not have operands, setting the appropriate values.
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
    /* todo - error handling */
    return 0;
}


/**
 * Function: handle_operation
 * Purpose: Handles the parsing and processing of an operation line in the assembly code.
 *
 * Parameters:
 *   work_line - The line containing the operation to parse.
 *   status - Pointer to the current status of the assembler.
 *   table - Pointer to the intern table.
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
    int op_size;
    char *source_label, *dest_label;
    static memory_word temp[MAX_OPERATION_LEN];
    for (i = 0; i < MAX_OPERATION_LEN; i++) {
        temp[i].data.value = 0;
    }
    /*set A to 1 for the first word of the operation*/
    temp->operation.A = 1;
    op_size = parse_operation(work_line, line_number, temp, status,
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