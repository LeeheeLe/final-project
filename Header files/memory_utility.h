
#ifndef MEMORY_UTILITY_H
#define MEMORY_UTILITY_H

#include <stdio.h>
#include "../Header Files/tables.h"
//todo: why do we need to treat label pointers differently?

typedef struct Memory_node {
    void *ptr;
    struct Memory_node *next_node;
} Memory_node;

//todo: again, why do we need to treat different pointers differently here?
typedef struct Line {
    FILE *file;
    char *file_name;
    char *line;
    int line_num;
    label_node *label;
} Line;


void *safe_alloc(size_t size);

void free_ptr(const void *ptr);

void free_all_memory();


#endif /*MEMORY_UTILITY_H*/