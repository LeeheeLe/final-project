#include "../Header Files/memory_utility.h"
#include "../Header Files/errors.h"

#include <stdlib.h>

static Memory_node *head = NULL;

void *safe_alloc(const size_t size) {
    Memory_node *new_node;
    void *ptr = malloc(size);
    if (ptr == NULL) {
        MEM_ALOC_ERROR();
        free_all_memory();
        exit(EXIT_FAILURE);
    }
    new_node = (Memory_node *)malloc(sizeof(Memory_node));
    if (new_node == NULL) {
        MEM_ALOC_ERROR();
        free(ptr);
        free_all_memory();
        exit(EXIT_FAILURE);
    }
    new_node->ptr = ptr;
    new_node->next_node = head;
    head = new_node;
    return ptr;
}

void free_ptr(const void *ptr) {
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