#include "../Header Files/memory_utility.h"
#include "../Header Files/errors.h"
#include "../Header Files/preprocessor.h"
#include "../Header Files/tables.h"

static Memory_node *head = NULL;


void free_label_table(label_node *node) {
        if (node == NULL) return;
        free_label_table(node->left);
        free_label_table(node->right);
        free((char *)node->name);
        free(node);
}

void *allocate_memory(long size) {
    Memory_node *new_node;
    void *ptr = malloc(size);
    if (ptr == NULL) {
        MEM_ALOC_ERROR();
        free_label_table(root); ////todo:where do we define root?
        free_all_memory();
        return NULL;
    }
    new_node = (Memory_node *)malloc(sizeof(Memory_node));
    if (new_node == NULL) {
        MEM_ALOC_ERROR();
        free(ptr);
        free_label_table(root);
        free_all_memory();
        return NULL;
    }
    new_node->ptr = ptr;
    new_node->next_node = head;
    head = new_node;
    return ptr;
}

void deallocate_memory(void *ptr) {
    Memory_node *current = head;
    Memory_node *prev = NULL;

    while (current != NULL) {
        if (current->ptr == ptr) {
            if (prev == NULL) {
                head = current->next_node;
            } else {
                prev->next_node = current->next_node;
            }
            free(current->ptr);
            free(current);
            return;
        }
        prev = current;
        current = current->next_node;
    }
}

void free_all_memory() {
    Memory_node *current = head;
    Memory_node *temp;
    while (current != NULL) {
        temp = current;
        current = current->next_node;
        free(temp->ptr);
        free(temp);
    }
    head = NULL;
}