#ifndef LABEL_TABLE_H
#define LABEL_TABLE_H
#include "errors.h"
#include <stddef.h>
#include <stdlib.h>

typedef enum {
  DATA, CODE
} label_data_type;

typedef enum {
  DEFAULT, EXTERN, ENTRY
} linking_type;

typedef struct node{
  const char *name;
  int value;
  label_data_type type;
  linking_type linking_type;
  struct node *right;
  struct node *left;
} label /*label is a node*/;

typedef struct {
  label *root;
} table_head;



#endif /*LABEL_TABLE_H*/
