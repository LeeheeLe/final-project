#ifndef LABEL_TABLE_H
#define LABEL_TABLE_H
#include "errors.h"
#include <stddef.h>
#include <stdlib.h>

typedef enum {
  DATA, CODE
} label_data_type;

typedef enum {
  DEFAULT, EXTERN
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

/* Initialises an empty table and allocates it dynamically
 *
 * returns: a pointer to the table
 */
table_head *initialise_table();

/* Adds a label to the table assuming that the label name is not taken
 *
 * param: head - the table to add the label to
 * param: name - the name of the label
 * param: value - the value of the label
 * param: type - the type of label (DATA/CODE)
 * param: linking_type - the linking type of the label (DEFAULT/EXTERN)
 */
void add_label(table_head *head, const char *name, int value,
               label_data_type type, linking_type linking_type);

/* Searches the table for a label matching the name provided
 *
 * param: name - the name of the label to search for
 * param: head - the table to search in
 *
 * returns: a pointer to the label object containing the name and value of the label found
 *  or null if no matching label was found
 */
label *find_label(const char *name, table_head head);


#endif /*LABEL_TABLE_H*/
