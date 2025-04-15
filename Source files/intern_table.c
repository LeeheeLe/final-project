#include <errors.h>
#include <tables.h>
#include <memory_utility.h>
#include <stdio.h>
#include <string.h>

/*
 * Purpose:
 * This file defines functions and structures for managing interned labels in
 * the assembler project, particularly focusing on code and data labels. It
 * includes functions for creating new interned labels, finding an interned
 * label by name, passing on the interns table, adding intern to an existing
 * intern table, and freeing the list of interned labels.
 *
 * Key Structures:
 * - `Intern`: A structure to hold interned labels with a name, type (code or
 * data), memory location, and a pointer to the next interned label in a linked
 * list.
 *
 * Key Functions:
 * - `str_dup`: Duplicates a string by allocating memory and copying the
 * content.
 * - `new_intern`: Creates a new interned label and returns a pointer to it.
 * - `find_intern`: Searches for an interned label by name in a linked list.
 * - `free_list`: Frees the memory used by the interned labels linked list.
 */

/**
 * Function: add_intern
 * Purpose: Creates a new interned label with a given name, type, and memory location.
 *
 * This function dynamically allocates memory for a new interned label and initializes
 * its fields (name, type, memory location, and next pointer). It returns a pointer to
 * the newly created interned label.
 *
 * Parameters:
 *   - char* name: The name of the interned label.
 *   - intern_type type: The type of the interned label (code or data).
 *   - int mem_place: The memory location of the interned label.
 *   - intern_node table: A pointer to the head of the intern table.
 *
 * Returns:
 *   - intern_node*: A pointer to the newly created interned label.
 */
intern_node* add_intern(char* name, intern_type type, int mem_place, intern_node table) {
  intern_node* new_intern = (intern_node*)safe_alloc(sizeof(intern_node));  /* Allocate memory for the new interned label. */
  new_intern->name = name;  /* Duplicate the name of the interned label. */
  new_intern->type = type;  /* Set the type (code or data) of the interned label. */
  new_intern->mem_place = mem_place;  /* Set the memory location of the interned label. */
  new_intern->next_intern = NULL;  /* Initialize the next pointer to NULL, indicating the end of the list. */
  return new_intern;  /* Return the newly created interned label. */
}

/**
 * Function: add_intern_node
 * Purpose: Adds an interned node to the end of the interned label list.
 *
 * Parameters:
 *   - intern_node* node: The interned node to be added.
 *   - intern_node* curr: The current intern node in the list.
 */
void add_intern_node(intern_node *node, intern_node *curr) {
    while (curr->next_intern != NULL) {
        curr = curr->next_intern;
    }
    curr->next_intern = node;
}

/**
 * Function: add_new_intern
 * Purpose: Adds a new interned label to the intern table.
 *
 * This function creates a new interned label and appends it to the intern table.
 *
 * Parameters:
 *   - intern_table_head* head: The head of the intern table.
 *   - const char* name: The name of the interned label.
 *   - int mem_place: The memory location of the interned label.
 *   - intern_type type: The type of the interned label (code or data).
 */
void add_new_intern(intern_table_head *head, char *name, int mem_place, intern_type type) {
  intern_node *node = safe_alloc(sizeof(intern_node));
  node->name = name;
  node->mem_place = mem_place;
  node->type = type;
  node->next_intern = NULL;
  if (head->root == NULL) {
    head->root = node;
    return;
  }
  add_intern_node(node, head->root);
}

/**
 * Function: initialise_intern_table
 * Purpose: Initializes an intern table.
 *
 * This function creates an intern table structure and initializes its root pointer
 * to NULL.
 *
 * Parameters:
 *   - None
 *
 * Returns:
 *   - intern_table_head*: A pointer to the initialized intern table.
 */
intern_table_head *initialise_intern_table() {
  intern_table_head *root = safe_alloc(sizeof(intern_table_head));
  root->root = NULL;
  return root;
}

/**
 * Function: check_interns_in_labels
 * Purpose: Verifies all interns against the label table.
 *
 * This function traverses the intern list and checks if each interned label
 * exists in the label table. It returns 1 if at least one intern is found.
 * If any intern is not found, it calls an error handler but continues checking the rest.
 *
 * Parameters:
 *   - intern_table_head* intern_head: Pointer to the intern table.
 *   - label_table_head* label_head: Pointer to the label table.
 *
 * Returns:
 *   - int: 1 if at least one intern is found in the label table, 0 otherwise.
 */
int check_interns_in_labels(intern_table_head *intern_head, label_table_head *label_head) {
    intern_node *current = intern_head->root;
    int found_interns = 0;

    while (current != NULL) {
        label_node *label = find_label(current->name, *label_head);
        if (label != NULL) {
            found_interns = 1;
        } else {
            NON_EXISTANT_NAME(current->name); /* missing intern todo: correct error, this one is for missing operations*/
        }
        current = current->next_intern;
    }

    return found_interns;
}