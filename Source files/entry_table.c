#include <tables.h>
#include <memory_utility.h>
#include <string.h>

/*
 * Function: add_entry
 * -------------------
 * Allocates memory for a new entry node, initializes it, and returns it.
 *
 * This function is used to create a new entry node for an entry label. The
 * node's name is set to the provided name, and the next pointer is set to NULL,
 * indicating the end of the list.
 *
 * Parameters:
 *   - name: The name of the entry label to be added.
 *   - table: The entry table (not used in this function, as the table will be updated separately).
 *
 * Returns:
 *   - A pointer to the newly created entry node.
 */
entry_node *add_entry(char *name, entry_node table) {
  entry_node* new_entry = (entry_node*)safe_alloc(sizeof(entry_node));  /* Allocate memory for the new entry node. */
  new_entry->name = name;  /* Set the name of the entry node. */
  new_entry->next_entry = NULL;  /* Initialize the next pointer to NULL, signifying the end of the list. */
  return new_entry;  /* Return the newly created entry node. */
}

/*
 * Function: add_entry_node
 * -------------------------
 * Adds an existing entry node to the end of an entry list.
 *
 * This function iterates through the current list of entry nodes to find the last
 * node and appends the given node at the end of the list.
 *
 * Parameters:
 *   - node: The entry node to be added to the list.
 *   - curr: The current entry node (used to traverse the list to the last node).
 */
void add_entry_node(entry_node *node, entry_node *curr) {
    while (curr->next_entry != NULL) {  /* Iterate until the last node. */
        curr = curr->next_entry;
    }
    curr->next_entry = node;  /* Append the node to the end of the list. */
}

/*
 * Function: add_new_entry
 * ------------------------
 * Adds a new entry to the entry table.
 *
 * This function creates a new entry node using `add_entry` and appends it to the
 * entry table's list. If the list is empty, the new node becomes the root of the list.
 *
 * Parameters:
 *   - head: The head of the entry table.
 *   - name: The name of the entry to be added.
 */
void add_new_entry(entry_table_head *head, char *name) {
  entry_node *node = safe_alloc(sizeof(entry_node));  /* Allocate memory for the new entry node. */
  node->name = name;  /* Set the name of the new entry node. */
  node->next_entry = NULL;  /* Initialize the next pointer to NULL. */

  if (head->root == NULL) {  /* If the list is empty, set the new node as the root. */
    head->root = node;
    return;
  }
  add_entry_node(node, head->root);  /* Otherwise, append the node to the existing list. */
}

/*
 * Function: initialise_entry_table
 * --------------------------------
 * Initializes a new entry table by allocating memory for it and setting its root to NULL.
 *
 * This function creates a new entry table that has an empty list (root is NULL).
 *
 * Returns:
 *   - A pointer to the newly created entry table.
 */
entry_table_head *initialise_entry_table() {
  entry_table_head *root = safe_alloc(sizeof(entry_table_head));  /* Allocate memory for the entry table. */
  root->root = NULL;  /* Initialize the root of the entry table to NULL. */
  return root;  /* Return the newly initialized entry table. */
}