#ifndef CONST_TABLES_H
#define CONST_TABLES_H
#include "mem_image.h"

struct operand_type {/**type of allowed operands for an operation**/
  uint IMMEDIATE : 1;
  uint ADDRESS : 1;
  uint RELATIVE : 1;
  uint REGISTER : 1;
};

typedef struct operation {
  char *name;
  int opcode;
  int funct;
  struct operand_type source_type;
  struct operand_type destination_type;
} operation_syntax;

operation_syntax find_operation(char *name);

#endif /*CONST_TABLES_H*/
