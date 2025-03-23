#ifndef CONST_TABLES_H
#define CONST_TABLES_H

typedef enum bool { false = 0, true = 1 } bool;

struct operand_type {
  bool IMMEDIATE;
  bool ADDRESS;
  bool RELATIVE;
  bool REGISTER;
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
