#ifndef HANDLE_TEXT_H
#define HANDLE_TEXT_H

#include <ctype.h>

int handle_numbers(char *line, int line_number, enum errors *status, memory data_image, int DC);
void handle_data_instruction(int *DC, memory data_image, enum errors *status,
                             char *work_line, inst instruction_type, int line_number);
void handle_instruction(int *DC, memory *data_image, enum errors *status, label_table_head *label_table,
                        entry_table_head *entry_table, char *work_line, char **label_name, inst instruction_type,
                        int line_number, int label_flag);
int handle_no_operand_operation(memory_word *temp, char **source_label, char **dest_label,
                                char *line, operation_syntax syntax, int *value1);
int handle_operation(char **work_line, enum errors *status, intern_table_head *table, int line_number,
                     memory code_image, const int IC);

#endif /*HANDLE_TEXT_H*/