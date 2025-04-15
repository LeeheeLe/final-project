#ifndef TABLES_H
#define TABLES_H

#include "errors.h"
#include <stddef.h>
#include <stdlib.h>

/*
 * Enum: label_data_type
 * ---------------------
 * Defines the types of labels in the assembler (DATA, CODE, EXTERNAL).
 * - DATA: Labels that are associated with data.
 * - CODE: Labels that are part of the executable code.
 * - EXTERNAL: Labels that are externals (linked from other modules).
 */
typedef enum { DATA, CODE, EXTERNAL } label_data_type;

/*
 * Enum: linking_type
 * ------------------
 * Defines the linking type of labels.
 * - DEFAULT: Default type of label, typically used in code or data.
 * - EXTERN: Labels that are externally linked.
 */
typedef enum { DEFAULT, EXTERN } linking_type;

/*
 * Structure: label_node
 * ---------------------
 * A node that represents a label in the label table.
 * Each label node contains the label's name, value, type, and linking type.
 */
typedef struct node {
  const char *name;                    /* The name of the label */
  int value;                           /* The value associated with the label */
  label_data_type type;                /* Type of label (DATA, CODE, EXTERNAL) */
  linking_type linking_type;           /* Linking type (DEFAULT or EXTERN) */
  struct node *right;                  /* Right child node in binary tree */
  struct node *left;                   /* Left child node in binary tree */
} label_node;

/* Label table head - points to the root of the binary search tree of labels */
typedef struct {
  label_node *root;                   /* Root of the label binary search tree */
} label_table_head;

/**
 * @brief Initialises an empty label table and allocates it dynamically.
 *
 * @return A pointer to the dynamically allocated label table.
 */
label_table_head *initialise_label_table();

/**
 * @brief Adds a new label to the label table assuming that the label name is not already taken.
 *
 * @param head the table to add the label to
 * @param name the name of the label
 * @param value the value of the label
 * @param type the type of label (DATA or CODE)
 * @param linking_type the linking type of the label (DEFAULT or EXTERN)
 */
void add_label(label_table_head *head, const char *name, int value,
               label_data_type type, linking_type linking_type);

/**
 * @brief Searches the label table for a label matching the name provided.
 *
 * @param name the name of the label to search for
 * @param head the label table to search in
 *
 * @return A pointer to the label object containing the name and value of the label
 *         or NULL if no matching label was found.
 */
label_node *find_label(const char *name, label_table_head head);

/*
 * Enum: intern_type
 * -----------------
 * Defines the types of interned labels (immediate or relative).
 * - immediate: The label is inserted with an immediate reference in memory.
 * - relative: The label is inserted with a relative reference in memory.
 */
typedef enum { immediate, relative } intern_type;

/*
 * Structure: intern_node
 * ----------------------
 * A node representing an interned label. Interned labels are used to resolve
 * addresses during the assembly process (either immediate or relative).
 */
typedef struct intern {
  char *name;                  /* The name of the interned label */
  intern_type type;            /* The type of interned label (immediate or relative) */
  int mem_place;               /* The memory location associated with the interned label */
  struct intern *next_intern;  /* Pointer to the next intern node */
} intern_node;

/* Intern table head - points to the root of the interned labels list */
typedef struct {
  intern_node *root;           /* Root of the interned labels list */
} intern_table_head;

/**
 * @brief Initialises an empty intern table and allocates it dynamically.
 *
 * @return A pointer to the dynamically allocated intern table.
 */
intern_table_head *initialise_intern_table();

/**
 * @brief Adds a new interned label to the intern table.
 *
 * @param name the name of the interned label
 * @param type the type of interned label (immediate or relative)
 * @param mem_place the memory place associated with the interned label
 * @param table the intern table to add the interned label to
 *
 * @return A pointer to the newly added interned label.
 */
intern_node *add_intern(char *name, intern_type type, int mem_place, intern_node table);

/**
 * @brief Adds a new interned label to the intern table.
 *
 * @param head the intern table to add the interned label to
 * @param name the name of the interned label
 * @param mem_place the memory place associated with the interned label
 * @param type the type of interned label (immediate or relative)
 */
void add_new_intern(intern_table_head *head, char *name, int mem_place, intern_type type);

/**
 * @brief Checks for the presence of interned labels in the label table.
 *
 * @param intern_head the intern table to check
 * @param label_head the label table to check against
 *
 * @return 1 if intern labels are present in the label table, 0 otherwise.
 */
int check_interns_in_labels(intern_table_head *intern_head, label_table_head *label_head);

/*
 * Structure: entry_node
 * ---------------------
 * Represents an entry in the entry table. Each entry node contains the name of the entry.
 */
typedef struct entry {
  char *name;                    /* The name of the entry */
  struct entry *next_entry;       /* Pointer to the next entry node */
} entry_node;

/* Entry table head - points to the root of the entry list */
typedef struct {
  entry_node *root;               /* Root of the entry list */
} entry_table_head;

/**
 * @brief Initialises an empty entry table and allocates it dynamically.
 *
 * @return A pointer to the dynamically allocated entry table.
 */
entry_table_head *initialise_entry_table();

/**
 * @brief Adds a new entry to the entry table.
 *
 * @param name the name of the entry to add
 * @param table the entry table to add the entry to
 *
 * @return A pointer to the newly added entry node.
 */
entry_node *add_entry(char *name, entry_node table);

/**
 * @brief Adds an entry node to the entry table.
 *
 * @param node the entry node to add
 * @param curr the current entry node in the table
 */
void add_entry_node(entry_node *node, entry_node *curr);

/**
 * @brief Adds a new entry to the entry table.
 *
 * @param head the entry table to add the entry to
 * @param name the name of the entry to add
 */
void add_new_entry(entry_table_head *head, char *name);

#endif /*TABLES_H*/