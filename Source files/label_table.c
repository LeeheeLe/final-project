#include "../Header files/label_table.h"
#include <stdlib.h>
#include <string.h>

/*
 * Purpose:
 * This file defines functions and structures for managing labels in the assembler project,
 * including functionality for creating a label table, adding labels to the table,
 * and searching for labels by their name.
 *
 * Key Structures:
 * - `table_head`: A structure representing the root of a binary search tree for labels.
 * - `label`: A structure representing an individual label with fields such as name, value,
 *   type, and linking type.
 *
 * Key Functions:
 * - `initialise_table`: Initializes a new label table.
 * - `node_cmp`: Compares two labels by their names.
 * - `add_node`: Adds a new label node to the binary search tree of labels.
 * - `add_label`: Adds a label to the table, initializing the label node with given data.
 * - `find_node`: Searches for a label node by name in the binary search tree.
 * - `find_label`: Finds a label by its name in the label table.
 */

/*
 * Purpose:
 * Initializes a new label table with a NULL root.
 *
 * @return table_head* A pointer to the newly initialized label table.
 */
table_head *initialise_table() {
  table_head *root = malloc(sizeof(table_head));
  root->root = NULL;
  return root;
}

/*
 * Purpose:
 * Compares two label nodes by their names.
 *
 * @param node1 The first label node.
 * @param node2 The second label node.
 *
 * @return int Result of strcmp between the names of node1 and node2.
 */
int node_cmp(label node1, label node2) {
  return strcmp(node1.name, node2.name);
}

/*
 * Purpose:
 * Recursively adds a new label node to the correct position in the binary search tree
 * of labels based on the comparison of names.
 *
 * @param node The label node to be added.
 * @param curr The current label node in the tree where the new node will be inserted.
 */
void add_node(label *node, label *curr) {
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
    add_node(node, curr->right);
  } else {
    if (curr->left == NULL) {
      curr->left = node;
      return;
    }
    add_node(node, curr->left);
  }
}

/*
 * Purpose:
 * Creates a new label node with the given data and adds it to the label table.
 * If the root is NULL, the node becomes the root; otherwise, it is added
 * to the appropriate position in the tree.
 *
 * @param head The label table to which the label is being added.
 * @param name The name of the label.
 * @param value The value associated with the label.
 * @param type The type of the label (like code or data).
 * @param linking_type The type of linking (like external or internal).
 */
void add_label(table_head *head, const char *name, int value,
               label_data_type type, linking_type linking_type) {
  label *node = malloc(sizeof(label));
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
  add_node(node, head->root);
}

/*
 * Purpose:
 * Recursively searches for a label node by name in the binary search tree.
 *
 * @param name The name of the label being searched for.
 * @param node The current node being checked.
 *
 * @return label* A pointer to the label node if found, NULL otherwise.
 */
label *find_node(const char *name, label *node) {
  if (node == NULL) {
    return node;
  }
  int compare = strcmp(name, node->name);
  if (compare == 0) {
    return node;
  }
  if (compare >= 0) {
    return find_node(name, node->right);
  }
  return find_node(name, node->left);
}

/*
 * Purpose:
 * Finds a label in the label table by its name by calling the recursive
 * search function `find_node`.
 *
 * @param name The name of the label being searched for.
 * @param head The label table being searched.
 *
 * @return label* A pointer to the label node if found, NULL otherwise.
 */
label *find_label(const char *name, table_head head) {
  return find_node(name, head.root);
}