#include "../Header files/const_tables.h"
#include "../Header files/errors.h"
#include <stdio.h>
#include <string.h>

static const operation_syntax no_operation = {NULL, -1, -1, {0,0,0,0}, {0,0,0,0}};
static const operation_syntax operations[] = {
    {"mov", 0, 0, {1, 1, 0, 1}, {0, 1, 0, 1}},
    {"cmp", 1, 0, {1, 1, 0, 1}, {1, 1, 0, 1}},
    {"add", 2, 1, {1, 1, 0, 1}, {0, 1, 0, 1}},
    {"sub", 2, 2, {1, 1, 0, 1}, {0, 1, 0, 1}},
    {"lea", 4, 0, {0, 1, 0, 0}, {0, 1, 0, 1}},
    {"clr", 5, 1, {0, 0, 0, 0}, {0, 1, 0, 1}},
    {"not", 5, 2, {0, 0, 0, 0}, {0, 1, 0, 1}},
    {"inc", 5, 3, {0, 0, 0, 0}, {0, 1, 0, 1}},
    {"dec", 5, 4, {0, 0, 0, 0}, {0, 1, 0, 1}},
    {"jmp", 9, 1, {0, 0, 0, 0}, {0, 1, 1, 0}},
    {"bne", 9, 2, {0, 0, 0, 0}, {0, 1, 1, 0}},
    {"jsr", 9, 3, {0, 0, 0, 0}, {0, 1, 1, 0}},
    {"red", 12,0, {0, 0, 0, 0}, {0, 1, 0, 3}},
    {"prn", 13,0, {0, 0, 0, 0}, {1, 1, 0, 1}},
    {"rts", 14,0, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {"stop",15,0, {0, 0, 0, 0}, {0, 0, 0, 0}}};
const int num_of_operations = sizeof(operations) / sizeof(operation_syntax);

/*input name, output opcode*/
operation_syntax find_operation(char *name) {
  int i;
  for (i = 0; i < num_of_operations; i++) {
    if (strcmp(operations[i].name, name) == 0) {
      return operations[i];
    }
  }
  NON_EXISTANT_NAME(name);
  return no_operation; /*name does not exist*/
}