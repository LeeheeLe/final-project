#include "../Header files/tables.h"
#include "../Header files/errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Purpose:
 * This file defines functions and structures for managing interned labels in the
 * assembler project, particularly focusing on code and data labels. It includes
 * functions for creating new interned labels, finding an interned label by name,
 * passing on the interns table, adding intern to an existing intern table and freeing
 * the list of interned labels.
 *
 * Key Structures:
 * - `Intern`: A structure to hold interned labels with a name, type (code or data),
 *   memory location, and a pointer to the next interned label in a linked list.
 *
 * Key Functions:
 * - `str_dup`: Duplicates a string by allocating memory and copying the content.
 * - `new_intern`: Creates a new interned label and returns a pointer to it.
 * - `find_intern`: Searches for an interned label by name in a linked list.
 * - `free_list`: Frees the memory used by the interned labels linked list.
 */


/**
 * Title: Creates a new interned label with a given name, type, and memory location.
 *
 * Purpose:
 * This function dynamically allocates memory for a new interned label and initializes
 * its fields (name, type, memory location, and next pointer). It returns a pointer to
 * the newly created interned label.
 */

intern_node* add_intern(char* name, intern_type type, int mem_place, intern_node table) {
  intern_node* new_intern = (intern_node*)malloc(sizeof(intern_node));  /* Allocate memory for the new interned label. */
  if (!new_intern) {
    MEM_ALOC_ERROR();
    return NULL;
}
  new_intern->name = strdup(name);  /* Duplicate the name of the interned label. */
  new_intern->type = type;  /* Set the type (code or data) of the interned label. */
  new_intern->mem_place = mem_place;  /* Set the memory location of the interned label. */
  new_intern->next_intern = NULL;  /* Initialize the next pointer to NULL, indicating the end of the list. */
  return new_intern;  /* Return the newly created interned label. */
}


void add_intern_node(intern_node *node, intern_node *curr) {
    while (curr->next_intern != NULL) {
        curr = curr->next_intern;
    }
    curr->next_intern = node;
}


void add_new_intern(intern_table_head *head, const char *name, int mem_place,intern_type type) {
  intern_node *node = malloc(sizeof(intern_node));
  node->name = strdup(name);
  node->mem_place = mem_place;
  node->type = type;
  node->next_intern = NULL;
  if (head->root == NULL) {
    head->root = node;
    return;
  }
  add_intern_node(node, head->root);
}



/*
 * Title: Frees all interned labels from memory.
 *
 * Purpose:
 * This function traverses the linked list of interned labels and frees the
 * memory allocated for each interned label. It ensures that all memory used
 * by the interned labels is properly released when no longer needed.
 */
void free_intern_list(intern_node* head) {
  intern_node* current = head;  /* Start from the head of the linked list. */
  intern_node* next_intern;

  while (current != NULL) {  /* Traverse the linked list. */
    next_intern = current->next_intern;  /* Save the pointer to the next interned label. */
    free(current->name);
    free(current);/* Free the current interned label. */
    current = next_intern;  /* Move to the next interned label in the list. */
  }
}

intern_table_head *initialise_intern_table() {
  intern_table_head *root = malloc(sizeof(intern_table_head));
  root->root = NULL;
  return root;
}


/**
 * Title: Verifies all interns against the label table, returns 1 if at least one matches.
 *
 * Purpose:
 * This function traverses the intern list and checks if each interned label
 * exists in the label table. It returns 1 if at least one intern is found.
 * If any intern is not found, it calls an error handler but continues checking the rest.
 *
 * @param intern_head Pointer to the intern table.
 * @param label_head Pointer to the label table.
 * @return 1 if at least one intern is found in the label table, 0 otherwise.
 */
int check_interns_in_labels(intern_table_head *intern_head, label_table_head *label_head) {
    intern_node *current = intern_head->root;
    int found_interns = 0;

    while (current != NULL) {
        label_node *label = find_label(current->name, *label_head);
        if (label != NULL) {
            found_interns = 1;
        } else {
            NON_EXISTANT_NAME(current->name); /*missing intern*/
        }
        current = current->next_intern;
    }

    return found_interns;
}
