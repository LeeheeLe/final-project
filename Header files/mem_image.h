#ifndef MEM_IMAGE_H
#define MEM_IMAGE_H
#define START_ADDRESS 100
#define MAX_MEM_SIZE 1<<10
typedef unsigned int uint;

typedef enum {
  IMMEDIATE = 0,
  DIRECT = 1,
  RELATIVE = 2,
  REGISTER = 3
} operand_type;

typedef struct {
  uint E : 1;
  uint R : 1;
  uint A : 1;
  uint funct : 5;
  uint dest_reg : 3;
  operand_type dest_type : 2;
  uint source_reg : 3;
  operand_type source_type : 2;
  uint opcode : 6;
} operation;

typedef struct {
  uint E : 1;
  uint R : 1;
  uint A : 1;
  uint value : 21;
} operand;


typedef struct {
  uint value : 24;
} data;

typedef struct {
  char value;
} character;

typedef union {
  operation operation;
  data data;
  character character;
  operand operand;
} memory_word;

typedef memory_word memory[MAX_MEM_SIZE];

#endif /* MEM_IMAGE_H */
