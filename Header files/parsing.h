

#ifndef PARSING_H
#define PARSING_H
#include "errors.h"

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

#define STR_INDICATOR '"'
#define DEFAULT_EXTERN_VALUE 0

#define MAX_LABEL_LENGTH 32

int is_instruction(char **line, inst *instruction_type, int line_number);
int is_whitespace(const char *line);
int is_comment(const char *line);
int is_label(char **line, char **label_name);
char *parse_string(char *line, int line_number, enum errors *status);
char *parse_linking_instruction(char *line, int line_number, enum errors *status);

#endif /*PARSING_H*/
