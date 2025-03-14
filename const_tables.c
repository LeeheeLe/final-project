#include "const_tables.h"
#include <stdio.h>
#include <stdlib.h>
#include "errors.h"
#include <string.h>

typedef struct Node {
    char* name;
    int opcode;
    int funct;
    struct Node* next_node; /*pointer to next node in list*/
} Node;

Node* new_node(char* name, int opcode, int funct) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        MEM_ALOC_ERROR();
        exit(1);
    }
    new_node->name = strdup(name); /*duplicate string to avoid pointer errors*/
    new_node->opcode = opcode;
    new_node->funct = funct;
    new_node->next_node = NULL;

    return new_node;
}

/*input name, output opcode*/
int find_opcode(char* name, Node* head) {
  Node* current;
  for (current = head; current != NULL; current = current->next_node) {
    if (current->name == name) {
      return current->opcode;
    }
  }
  UNEXISTING_NAME(name);
  return -1; /*name does not exist*/
}

/*input name, output funct*/
int find_funct(char* name, Node* head) {
    Node* current;
    for (current = head; current != NULL; current = current->next_node) {
        if (current->name == name) {
            return current->funct;
        }
    }
    UNEXISTING_NAME(name);
    return -1; /*name does not exist*/
}

void free_list(Node* head) {
  Node* current = head;
  Node* next_node;
  while (current != NULL) {
    next_node = current->next_node;
    free(current);
    current = next_node;
  }
}

/*TODO: implement the values in the table - how?*/