#include "memory_utility.h"
#include "errors.h"
#include "parsing.h"
#include "first_pass.h"
#include "const_tables.h"
#include <stdlib.h>

static Memory_node *head = NULL;

/*
 * Purpose:
 * This file implements memory management functions that ensure safe memory allocation,
 * track allocated memory blocks, and provide cleanup functions to free memory and avoid leaks.
 * It uses a linked list (`Memory_node`) to track all allocated memory pointers for proper management.
 *
 * Key Functions:
 * - `safe_alloc`: Allocates memory for a given size and tracks the pointer.
 * - `free_ptr`: Frees a specific memory block and removes it from the tracking list.
 * - `free_all_memory`: Frees all allocated memory tracked by the list.
 */

/**
 * Title: Safe Memory Allocation
 *
 * Purpose:
 * This function allocates memory for the specified size and tracks the pointer in a linked list.
 * If the allocation fails, it prints an error message, frees any already allocated memory,
 * and terminates the program.
 *
 * @param size The size of memory to allocate.
 * @return void* A pointer to the allocated memory block.
 * @exit If memory allocation fails, the program is terminated with `exit(EXIT_FAILURE)`.
 */
void *safe_alloc(const size_t size) {
    Memory_node *new_node;
    void *ptr = malloc(size);

    /* If malloc fails to allocate memory, print error, clean up and exit.*/
    if (ptr == NULL) {
        MEM_ALOC_ERROR();  /* Error handling function (assumed to print an error message) */
        free_all_memory();
        exit(EXIT_FAILURE);
    }

    /* Allocate memory to track this block in the linked list.*/
    new_node = (Memory_node *)malloc(sizeof(Memory_node));

    /* If malloc fails to allocate memory for the node, handle error and exit.*/
    if (new_node == NULL) {
        MEM_ALOC_ERROR();
        free(ptr);
        free_all_memory();
        exit(EXIT_FAILURE);
    }

    /* Track the allocated memory in the linked list.*/
    new_node->ptr = ptr;
    new_node->next_node = head;
    head = new_node;

    return ptr;  /* Return the allocated memory pointer */
}

/**
 * Title: Free Memory Pointer
 *
 * Purpose:
 * This function frees a specific memory block, given its pointer, and removes it from the tracking list.
 * If the pointer is not found in the list, it does nothing.
 *
 * @param ptr The pointer to the memory block to free.
 */
void free_ptr(const void *ptr) {
    Memory_node *current = head;
    Memory_node *prev = NULL;

    /* Traverse the linked list to find the memory node that holds the specified pointer.*/
    while (current != NULL) {
        if (current->ptr == ptr) {  /* If the pointer matches Remove the node from the list*/
            if (prev == NULL) {
                head = current->next_node;  /* Remove from the head of the list.*/
            } else {
                prev->next_node = current->next_node;  /* Skip the current node.*/
            }
            free(current->ptr);  /* Free the allocated memory*/
            free(current);  /* Free the node that tracked this memory block*/
            return;  /* Memory successfully freed, exit the function.*/
        }
        prev = current;
        current = current->next_node;
    }
}

/**
 * Title: Free All Allocated Memory
 *
 * Purpose:
 * This function frees all the memory blocks that were allocated using `safe_alloc`,
 * as tracked by the linked list. After freeing the memory, it resets the list to NULL.
 */
void free_all_memory() {
    Memory_node *current = head;
    Memory_node *temp;

    /* Traverse the list, freeing all memory blocks and their tracking nodes.*/
    while (current != NULL) {
        temp = current;
        current = current->next_node;
        free(temp->ptr);  /* Free the actual allocated memory block*/
        free(temp);  /* Free the node tracking this memory*/
    }
    head = NULL;  /* Reset the linked list to indicate all memory is freed.*/
}