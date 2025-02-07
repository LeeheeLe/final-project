/*
 * Created by itay on 12/31/24.
 */

#ifndef INPUT_H
#define INPUT_H
#include <stdio.h>
#include <stdlib.h>

#define STARTING_LINE_SIZE 64

/*get line (arbitrary length) from file, returns a pointer to the start of the line*/
char *getLine(const char *file);

#endif
