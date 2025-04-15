
#ifndef MEMORY_UTILITY_H
#define MEMORY_UTILITY_H

#include <stdio.h>

typedef struct Memory_node {
    void *ptr;
    struct Memory_node *next_node;
} Memory_node;

void *safe_alloc(size_t size);

void free_ptr(const void *ptr);

void free_all_memory();


#endif /*MEMORY_UTILITY_H*/