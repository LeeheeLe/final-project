#ifndef MEMORY_UTILITY_H
#define MEMORY_UTILITY_H

#include <stdio.h>

/*
 * File: memory_utility.h
 * ----------------------
 * This header defines utility functions for managing dynamic memory
 * allocation and deallocation. It provides a safe memory allocation
 * mechanism and ensures that all dynamically allocated memory is
 * properly freed at the end of the program execution.
 *
 * Functions:
 *  - safe_alloc: Allocates memory and checks for allocation failure.
 *  - free_ptr: Frees a specific memory pointer.
 *  - free_all_memory: Frees all dynamically allocated memory.
 */

/*
 * Struct: Memory_node
 * -------------------
 * Represents a node in the memory tracking list.
 * Used for managing dynamically allocated memory pointers.
 *
 * Fields:
 *  ptr        - A pointer to the allocated memory.
 *  next_node  - A pointer to the next node in the linked list (for tracking).
 */
typedef struct Memory_node {
    void *ptr;                  /* Pointer to allocated memory */
    struct Memory_node *next_node; /* Next node in memory tracking list */
} Memory_node;

/*
 * Function: safe_alloc
 * --------------------
 * Allocates memory of the specified size and checks if allocation
 * was successful. If allocation fails, it prints an error message
 * and exits the program.
 *
 * Parameters:
 *  size - The number of bytes to allocate.
 *
 * Returns:
 *  A pointer to the allocated memory block.
 */
void *safe_alloc(size_t size);

/*
 * Function: free_ptr
 * ------------------
 * Frees a specific memory pointer.
 * It is part of the utility to ensure memory is freed manually.
 *
 * Parameters:
 *  ptr - A pointer to the memory block to be freed.
 */
void free_ptr(const void *ptr);

/*
 * Function: free_all_memory
 * --------------------------
 * Frees all dynamically allocated memory that has been tracked by the program.
 * This function should be called at the end of the program to avoid memory leaks.
 */
void free_all_memory();

#endif /* MEMORY_UTILITY_H */