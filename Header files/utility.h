#ifndef UTILITY_H
#define UTILITY_H

int is_data_instruction(inst instruction_type);
int is_linking_instruction(inst instruction_type);
int is_empty(op_type type);
int is_register(char *operand);


/*
 * Function: is_instruction
 * ------------------------
 * Checks if the line contains a valid instruction. It also sets the instruction
 * type and provides the line number for error reporting.
 *
 * Parameters:
 *  line              - A pointer to the line being analyzed.
 *  instruction_type  - A pointer to the variable where the instruction type will be stored.
 *  line_number       - The line number of the instruction for error reporting.
 *
 * Returns:
 *  1 if the line contains a valid instruction, 0 otherwise.
 */
int is_instruction(char **line, inst *instruction_type, int line_number);

/*
 * Function: is_whitespace
 * -----------------------
 * Checks if the line consists only of whitespace characters (spaces, tabs, etc.).
 *
 * Parameters:
 *  line - The line to check.
 *
 * Returns:
 *  1 if the line is empty or only contains whitespace, 0 otherwise.
 */
int is_whitespace(const char *line);

/*
 * Function: is_comment
 * --------------------
 * Checks if the line is a comment (starts with a comment character).
 *
 * Parameters:
 *  line - The line to check.
 *
 * Returns:
 *  1 if the line is a comment, 0 otherwise.
 */
int is_comment(const char *line);

/*
 * Function: is_label
 * ------------------
 * Checks if the line starts with a label. A label is followed by a colon
 * and can be placed at the beginning of a line.
 *
 * Parameters:
 *  line        - A pointer to the line being checked.
 *  label_name  - A pointer to the variable where the label name will be stored.
 *  line_number - The line number of the label for error reporting.
 *
 * Returns:
 *  1 if the line contains a valid label, 0 otherwise.
 */
int is_label(char **line, char **label_name, int line_number);

#endif /*UTILITY_H*/