#include "../Header files/input.h"
#include "../Header files/errors.h"

/*
 * Function: getLine
 * Reads a line of text from the given input file.
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
    line = (char *) malloc(line_size * sizeof(char));
    if (line == NULL) {
        MEM_ALOC_ERROR();
        return NULL;  /* Memory allocation failed */
    }

    /* Continuously read characters until a newline or EOF is encountered */
    while (1) {
        c = (char) fgetc(input);

        /* Check for error or End Of File */
        if (c == EOF) {
            if (current_idx == 0) {
                free(line);  /* If no characters were read, free memory and return NULL */
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
            new_line = realloc(line, line_size * sizeof(char));
            if (new_line == NULL) {
                free(line);
                free(new_line);  /* If realloc fails, free memory and handle the error */
                MEM_ALOC_ERROR();
                return NULL;
            }
            line = new_line;  /* Update the line pointer to the newly allocated memory */
        }
    }

    /* Null-terminate the string */
    line[current_idx] = '\0';
    return line;  /* Return the complete line */
}