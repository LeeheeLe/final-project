#include "../Header files/intern_table.h"
#include "../Header files/errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *str_dup(const char *src) {
  char *dest;
  dest = (char *)malloc(strlen(src) + 1);
  if (dest == NULL) {
    MEM_ALOC_ERROR();
    return NULL;
  }
  strcpy(dest, src);
  return dest;
}

enum intern_type {code, data};
typedef struct Intern {
  char *name;
  enum intern_type type;
  int mem_place;
  struct Intern *next_intern;
} Intern;

Intern* new_intern(char* name, enum intern_type type, int mem_place) {
  Intern* new_intern = (Intern*)malloc(sizeof(Intern));
  if (new_intern == NULL) {
    MEM_ALOC_ERROR();
    //todo free and return null or something
  }

  new_intern->name = str_dup(name);
  new_intern-> type = type;
  new_intern->mem_place = mem_place;
  new_intern->next_intern = NULL;
  return new_intern;
}

Intern find_intern(char *name, Intern* head) {
  Intern* current;
  for (current = head; current != NULL; current = current->next_intern) {
    if (strcmp(current->name, name)) {
      return *current;
    }
  }
  NON_EXISTANT_NAME(name);
  const Intern empty_intern = {NULL, -1, -1, NULL};
  return empty_intern; /*name does not exist*/
}

void free_list(Intern* head) {
  Intern* current = head;
  Intern* next_intern;
  while (current != NULL) {
    next_intern = current->next_intern;
    free(current);
    current = next_intern;
  }
}