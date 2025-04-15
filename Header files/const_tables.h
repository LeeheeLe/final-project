#ifndef CONST_TABLES_H
#define CONST_TABLES_H

struct operand_type {/**type of allowed operands for an operation**/
  int IMMEDIATE : 1;
  int ADDRESS : 1;
  int RELATIVE : 1;
  int REGISTER : 1;
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
