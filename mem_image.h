#ifndef MEM_IMAGE_H
#define MEM_IMAGE_H

#define MAX_MEM_SIZE 1<<21

typedef struct {
  int opcode : 6;
  int source_type : 2; // make an enum
  int source_reg : 3;
  int dest_type : 2; // mae enum
  int dest_reg : 3;
  int funct : 5;
  int A : 1;
  int R : 1;
  int E : 1;
} operation;

typedef struct {
  int value;
} number;

typedef union {
  operation op;
  number num;
} memory_word;

typedef memory_word memory[MAX_MEM_SIZE];

#endif /* MEM_IMAGE_H */
