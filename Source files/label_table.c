#include "tables.h"
#include "parsing.h"
#include "first_pass.h"
#include "const_tables.h"
#include "memory_utility.h"
#include <string.h>

/*
 * Purpose:
 * This file defines functions and structures for managing labels in the
 * assembler project, including functionality for creating a label table, adding
 * labels to the table, and searching for labels by their name.
 *
 * Key Structures:
 * - `table_head`: A structure representing the root of a binary search tree for
 * labels.
 * - `label`: A structure representing an individual label with fields such as
 * name, value, type, and linking type.
 *
 * Key Functions:
 * - `initialise_table`: Initializes a new label table.
 * - `node_cmp`: Compares two labels by their names.
 * - `add_node`: Adds a new label node to the binary search tree of labels.
 * - `add_label`: Adds a label to the table, initializing the label node with
 * given data.
 * - `find_node`: Searches for a label node by name in the binary search tree.
 * - `find_label`: Finds a label by its name in the label table.
 */

/**
 * Function: initialise_label_table
 * Purpose: Initializes a new label table with a NULL root.
 *
 * This function creates a label table structure and sets its root pointer
 * to `NULL`. This table will later hold label nodes organized in a binary
 * search tree.
 *
 * Parameters:
 *   - None
 *
 * Returns:
 *   - label_table_head*: A pointer to the newly initialized label table.
 */
label_table_head *initialise_label_table() {
  label_table_head *root = safe_alloc(sizeof(label_table_head));
  root->root = NULL;
  return root;
}

/**
 * Function: node_cmp
 * Purpose: Compares two label nodes by their names.
 *
 * This function compares the names of two label nodes using the `strcmp`
 * function. It returns the result of `strcmp` to determine their lexicographical order.
 *
 * Parameters:
 *   - label_node node1: The first label node.
 *   - label_node node2: The second label node.
 *
 * Returns:
 *   - int: Result of `strcmp` between the names of `node1` and `node2`.
 */
int node_cmp(label_node node1, label_node node2) {
  return strcmp(node1.name, node2.name);
}

/**
 * Function: add_label_node
 * Purpose: Recursively adds a new label node to the correct position in the binary search tree
 *          of labels based on the comparison of names.
 *
 * This function compares the new label node's name with the current node in the tree
 * and inserts the new node in the left or right subtree accordingly.
 *
 * Parameters:
 *   - label_node* node: The label node to be added.
 *   - label_node* curr: The current node in the tree where the new node will be inserted.
 */
void add_label_node(label_node *node, label_node *curr) {
  if (curr->left == NULL && curr->right == NULL) {
    if (node_cmp(*node, *curr) >= 0) {
      curr->right = node;
    } else {
      curr->left = node;
    }
    return;
  }
  if (node_cmp(*node, *curr) >= 0) {
    if (curr->right == NULL) {
      curr->right = node;
      return;
    }
    add_label_node(node, curr->right);
  } else {
    if (curr->left == NULL) {
      curr->left = node;
      return;
    }
    add_label_node(node, curr->left);
  }
}

/**
 * Function: add_label
 * Purpose: Creates a new label node with the given data and adds it to the label table.
 *
 * If the root of the table is `NULL`, the node becomes the root of the table.
 * Otherwise, it is added to the correct position in the binary search tree.
 *
 * Parameters:
 *   - label_table_head* head: The label table to which the label is being added.
 *   - const char* name: The name of the label.
 *   - int value: The value associated with the label.
 *   - label_data_type type: The type of the label (e.g., code or data).
 *   - linking_type linking_type: The type of linking (e.g., external or internal).
 */
void add_label(label_table_head *head, const char *name, int value,
               label_data_type type, linking_type linking_type) {
  label_node *node = safe_alloc(sizeof(label_node));
  node->name = name;
  node->value = value;
  node->type = type;
  node->linking_type = linking_type;
  node->left = NULL;
  node->right = NULL;
  if (head->root == NULL) {
    head->root = node;
    return;
  }
  add_label_node(node, head->root);
}

/**
 * Function: find_node
 * Purpose: Recursively searches for a label node by name in the binary search tree.
 *
 * This function traverses the binary search tree by comparing the given `name` with
 * the current node's name and recursively moves to the left or right subtree until
 * the node is found or the end of the tree is reached.
 *
 * Parameters:
 *   - const char* name: The name of the label being searched for.
 *   - label_node* node: The current node being checked.
 *
 * Returns:
 *   - label_node*: A pointer to the label node if found, `NULL` otherwise.
 */
label_node *find_node(const char *name, label_node *node) {
  int compare;
  if (node == NULL) {
    return node;
  }
  compare = strcmp(name, node->name);
  if (compare == 0) {
    return node;
  }
  if (compare >= 0) {
    return find_node(name, node->right);
  }
  return find_node(name, node->left);
}

/**
 * Function: find_label
 * Purpose: Finds a label in the label table by its name by calling the recursive
 *          search function `find_node`.
 *
 * This function is a wrapper around the `find_node` function to search for a label
 * in the label table by its name.
 *
 * Parameters:
 *   - const char* name: The name of the label being searched for.
 *   - label_table_head head: The label table being searched.
 *
 * Returns:
 *   - label_node*: A pointer to the label node if found, `NULL` otherwise.
 */
label_node *find_label(const char *name, label_table_head head) {
  return find_node(name, head.root);
}