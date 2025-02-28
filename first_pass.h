#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "errors.h"
#include "file_extensions.h"
#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
  DATA,
  STRING,
  ENTRY,
  EXTERN,
  INVALID
} inst;

typedef struct {
  inst instruction;
  char *name;
} instruction;

instruction instructions[] = {
  {DATA, "data"},
  {STRING, "string"},
  {ENTRY, "entry"},
  {EXTERN, "extern"}
};

#define NUMBER_OF_INSTRUCTION_TYPES 4

#define LABEL_DEF_CHAR ':'
#define INSTRUCTION_START_CHAR '.'

#define INPUT_EXT PREPROCESSOR_OUTPUT_EXT
/*the input of the first pass is the output of the preprocessor*/

void first_pass(const char *file_name);


#endif // FIRST_PASS_H
