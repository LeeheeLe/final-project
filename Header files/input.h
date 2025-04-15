#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <stdlib.h>

/*
 * File: input.h
 * -------------
 * This header defines utility functions for handling file input,
 * especially for reading source lines and handling file name extensions.
 *
 * Functions:
 *  - getLine: Dynamically reads a full line of any length from a file.
 *  - add_extension: Appends a file extension to a base filename.
 *
 * Constants:
 *  - STARTING_LINE_SIZE: Initial allocation size for reading lines.
 */

/*
 * Constant: STARTING_LINE_SIZE
 * ----------------------------
 * The initial buffer size allocated for reading a line from file.
 * The buffer is expanded dynamically if the line exceeds this size.
 */
#define STARTING_LINE_SIZE 64

/*
 * Function: getLine
 * -----------------
 * Reads a single line of arbitrary length from the input file.
 * The returned pointer is dynamically allocated and must be freed
 * by the caller.
 *
 * Parameters:
 *  input - A pointer to the file to read from.
 *
 * Returns:
 *  A dynamically allocated string containing the full line, or NULL on failure.
 */
char *getLine(FILE *input);

/*
 * Function: add_extension
 * -----------------------
 * Appends a file extension to a given base filename.
 * Allocates and returns a new string with the full name.
 *
 * Parameters:
 *  filename  - The base name of the file (e.g., "source")
 *  extension - The file extension to append (e.g., ".am")
 *
 * Returns:
 *  A new string with the combined filename and extension.
 *  The caller is responsible for freeing the returned string.
 */
char *add_extension(const char *filename, const char *extension);

#endif /* INPUT_H */
