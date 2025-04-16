#include "input.h"
#include "memory_utility.h"
#include "parsing.h"
#include "first_pass.h"
#include "const_tables.h"
#include <string.h>

/*
 * Function: getLine
 * Purpose: Reads a line of text from the given input file.
 *
 * This function dynamically allocates memory to store the characters of a line from a file.
 * It reads characters one by one until it encounters a newline or EOF (End of File).
 * If the allocated memory is not enough, the function doubles the memory and continues reading.
 * If an error occurs during memory allocation or file reading, the function handles it and returns NULL.
 *
 * Parameters:
 *   - FILE *input: The file pointer from which the line is to be read.
 *
 * Returns:
 *   - A pointer to a string (char*) containing the line of text, or NULL if an error occurs or EOF is reached.
 */
char *getLine(FILE *input) {
    /*
     * current_idx: Tracks the current position in the line.
     * c: Character read from the file.
     * line_size: The initial size of the line buffer.
     * line: Pointer to the line buffer to store the text.
     * new_line: Temporary pointer used for reallocating memory.
     */
    int current_idx = 0;
    char c;
    int line_size = STARTING_LINE_SIZE;
    char *line;
    char *new_line;

    /* Allocate initial memory for the line buffer */
    line = (char *) safe_alloc(line_size * sizeof(char));
    /* Continuously read characters until a newline or EOF is encountered */
    while (1) {
        c = (char) fgetc(input);

        /* Check for error or End Of File */
        if (c == EOF) {
            if (current_idx == 0) {
                free_ptr(line);  /* If no characters were read, free memory and return NULL */
                return NULL;
            }
            break;  /* EOF reached */
        }
        if (c == '\n') {
            break;  /* Newline character encountered */
        }

        /* Store the read character in the line buffer */
        line[current_idx++] = c;

        /* If buffer is full, allocate more memory */
        if (current_idx >= line_size) {
            line_size *= 2;  /* Double the buffer size */
            new_line = safe_alloc(line_size);
            memcpy(new_line, line, current_idx);
            free_ptr(line);
            line = new_line;  /* Update the line pointer to the newly allocated memory */
        }
    }

    /* Null-terminate the string */
    line[current_idx] = '\0';
    return line;  /* Return the complete line */
}

/**
 * Function: add_extension
 * Purpose: Adds an extension to a given filename string.
 *
 * This function takes a base filename and appends a specified extension to it.
 * It returns the newly constructed filename with the extension appended.
 *
 * Parameters:
 *   - const char *filename: The original filename to which the extension will be added.
 *   - const char *extension: The extension to be appended to the filename.
 *
 * Returns:
 *   - A new string (char*) containing the filename with the extension added.
 */
char *add_extension(const char *filename, const char *extension) {
    char *new_filename;
    const int filename_length = (int) strlen(filename);
    const int extension_length = (int) strlen(extension);

    new_filename = (char *)safe_alloc(filename_length + extension_length + 1);
    strcpy(new_filename, filename);
    strcat(new_filename, extension);
    return new_filename;
}