#ifndef TABLES_H
#define TABLES_H

#include "errors.h"
#include <stddef.h>
#include <stdlib.h>

typedef enum { DATA, CODE, EXTERNAL } label_data_type;

typedef enum { DEFAULT, EXTERN } linking_type;

typedef struct node {
  const char *name;
  int value;
  label_data_type type;
  linking_type linking_type;
  struct node *right;
  struct node *left;
} label_node /*label is a node*/;

typedef struct {
  label_node *root;
} label_table_head;

/**
 * @brief Initialises an empty table and allocates it dynamically
 *
 * @return a pointer to the table
 */
label_table_head *initialise_label_table();

/**
 * @brief Adds a label to the table assuming that the label name is not taken
 *
 * @param head the table to add the label to
 * @param name the name of the label
 * @param value the value of the label
 * @param type the type of label (DATA/CODE)
 * @param linking_type the linking type of the label (DEFAULT/EXTERN)
 */
void add_label(label_table_head *head, const char *name, int value,
               label_data_type type, linking_type linking_type);

/**
 * @brief Searches the table for a label matching the name provided
 *
 * @param name the name of the label to search for
 * @param head the table to search in
 *
 * @return a pointer to the label object containing the name and value of the
 * label found or null if no matching label was found
 */
label_node *find_label(const char *name, label_table_head head);

/**
 * Title: Enum definition for intern types (immediate or relative).
 *
 * Purpose:
 * This enum is used to define the type of an interned label. It indicates
 * whether the interned label should be inserted with an immediate reference or
 * a relative one in the assembler's memory.
 */

typedef enum { immediate, relative } intern_type;

typedef struct intern {
  char *name;
  intern_type type;
  int mem_place;
  struct intern *next_intern;
} intern_node;

typedef struct {
  intern_node *root;
} intern_table_head;

/**
 * @brief Initialises an empty table and allocates it dynamically
 *
 * @return a pointer to the table
 */
intern_table_head *initialise_intern_table();
intern_node *add_intern(char *name, intern_type type, int mem_place,
                        intern_node table);
void add_new_intern(intern_table_head *head, const char *name, int mem_place,
                    intern_type type);
void free_intern_list(intern_node *head);
intern_table_head *initialise_intern_table();
int check_interns_in_labels(intern_table_head *intern_head,
                            label_table_head *label_head);

typedef struct entry {
  char *name;
  struct entry *next_entry;
} entry_node;

typedef struct {
  entry_node *root;
} entry_table_head;

entry_table_head *initialise_entry_table();
entry_node *add_entry(const char *name, entry_node table);
void add_entry_node(entry_node *node, entry_node *curr);
void add_new_entry(entry_table_head *head, const char *name);
void free_entry_list(entry_node *head);
void iterate_entry(entry_table_head *entry_head);

#endif /*TABLES_H*/
