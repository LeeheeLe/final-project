#include "../Header Files/tables.h"
#include "../Header Files/errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Entry_node *add_entry(char* name, Entry_node table) {
  Entry_node* new_entry = (Entry_node*)malloc(sizeof(Intern_node));  /* Allocate memory for the new entry label. */
  if (!new_entry) {
    MEM_ALOC_ERROR();
    return NULL;
}
  new_entry->name = strdup(name);  /* Duplicate the name of the entry label. */
  new_entry->next_entry = NULL;  /* Initialize the next pointer to NULL, indicating the end of the list. */
  return new_entry;  /* Return the newly created entry label. */
}


void add_entry_node(Entry_node *node, Entry_node *curr) {
    while (curr->next_entry != NULL) {
        curr = curr->next_entry;
    }
    curr->next_entry = node;
}


void add_new_entry(entry_table_head *head, const char *name) {
  Entry_node *node = malloc(sizeof(Entry_node));
  node->name = strdup(name);
  node->next_entry = NULL;
  if (head->root == NULL) {
    head->root = node;
    return;
  }
  add_entry_node(node, head->root);
}


void free_entry_list(Entry_node* head) {
  Entry_node* current = head;
  Entry_node* next_entry;

  while (current != NULL) {
    next_entry = current->next_entry;
    free(current->name);
    free(current);
    current = next_entry;
  }
}

entry_table_head *initialise_entry_table() {
  entry_table_head *root = malloc(sizeof(entry_table_head));
  root->root = NULL;
  return root;
}