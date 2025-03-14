#include "label_table.h"

#include <stdlib.h>
#include <string.h>

table_head *initialise_table() {
  table_head *root = malloc(sizeof(table_head));
  root->root = NULL;
  return root;
}

int node_cmp(label node1, label node2) {
  return strcmp(node1.name, node2.name);
}

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

label *find_node(const char *name, label *node) {
  if (node == NULL) {
    return node;
  }
  int compare = strcmp(name, node->name);
  if (compare==0) {
    return node;
  }
  if (compare>=0) {
    return find_node(name, node->right);
  }
  return find_node(name, node->left);
}

label *find_label(const char *name, table_head head) {
  return find_node(name, head.root);
}