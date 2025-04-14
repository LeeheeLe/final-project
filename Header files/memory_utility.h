
#ifndef MEMORY_UTILITY_H
#define MEMORY_UTILITY_H

#include <stdio.h>
#include "../Header Files/tables.h"
typedef struct Memory_node {
    void *ptr;
    struct Memory_node *next_node;
} Memory_node;

typedef struct Line {
    FILE *file;
    char *file_name;
    char *line;
    int line_num;
    label_node *label;
} Line;


void *allocate_memory(long size);

void deallocate_memory(void *ptr);

void free_all_memory();


#endif /*MEMORY_UTILITY_H*/