#ifndef MEM_IMAGE_H
#define MEM_IMAGE_H

#define MAX_MEM_SIZE 1<<10
typedef unsigned int uint;

typedef struct {
  uint opcode : 6;
  uint source_type : 2; // make an enum
  uint source_reg : 3;
  uint dest_type : 2; // mae enum
  uint dest_reg : 3;
  uint funct : 5;
  uint A : 1;
  uint R : 1;
  uint E : 1;
} operation;

typedef struct {
  int value : 21;
  uint A : 1;
  uint R : 1;
  uint E : 1;
} operand;


typedef struct {
  int value : 24;
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
