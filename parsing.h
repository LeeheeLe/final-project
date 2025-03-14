

#ifndef PARSING_H
#define PARSING_H

typedef enum {
  DATA_INST,
  STRING_INST,
  ENTRY_INST,
  EXTERN_INST,
  INVALID_INST
} inst;

typedef struct {
  inst instruction;
  char *name;
} instruction;

 extern instruction instructions[];
#define NUMBER_OF_INSTRUCTION_TYPES 4


#define LABEL_DEF_CHAR ':'
#define INSTRUCTION_START_CHAR '.'

int is_instruction(char **line, inst *instruction_type, int line_number);
int is_whitespace(const char *line);
int is_comment(const char *line);
int is_label(char **line, char **label_name);

#endif /*PARSING_H*/
