#ifndef TABLES_H
#define TABLES_H

#include "errors.h"
#include <stddef.h>
#include <stdlib.h>

typedef enum {
    DATA, CODE, EXTERNAL
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
} Label_node /*label is a node*/;

typedef struct {
    Label_node *root;
} label_table_head;


/**
 * Title: Enum definition for intern types (code or data).
 *
 * Purpose:
 * This enum is used to define the type of an interned label. It indicates
 * whether the interned label is part of the code section or the data section
 * in the assembler's memory.
 */

typedef enum {
    code, data
} intern_type;

typedef struct intern {
    char *name;
    intern_type type;
    int mem_place;
    struct intern *next_intern;
} Intern_node;

typedef struct {
    Intern_node *root;
} intern_table_head;



/**
 * @brief Initialises an empty table and allocates it dynamically
 *
 * @return a pointer to the table
 */
label_table_head *initialise_label_table();

/**
 * @brief Initialises an empty table and allocates it dynamically
 *
 * @return a pointer to the table
 */
intern_table_head *initialise_intern_table();

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
 * @return a pointer to the label object containing the name and value of the label found
 *  or null if no matching label was found
 */
Label_node *find_label(const char *name, label_table_head head);

#endif /*TABLES_H*/
