#include "input.h"
#include "errors.h"

char *getLine(FILE *input) {
    int current_idx = 0;
    char c;
    int eof_flag = 0;
    int line_size = STARTING_LINE_SIZE;
    char *line;
    char *new_line;
    line = (char *) malloc(line_size * sizeof(char));
    if (line == NULL) {
        MEM_ALOC_ERROR();
        return NULL;
    }
    while (1) {
        c = (char) fgetc(input);

        /*check for error  or End Of File*/
        if (c == EOF) {
            eof_flag = 1;
            if (current_idx == 0) {
                free(line);
                return NULL;
            }
            break;
        }
        if (c == '\n') {
            break;
        }

        line[current_idx++] = c;

        /*if need more memory*/
        if (current_idx >= line_size) {
            line_size *= 2; /*double the memory*/
            new_line = realloc(line, line_size * sizeof(char));
            if (new_line == NULL) {
                free(line);
                free(new_line);
                MEM_ALOC_ERROR();
                return NULL;
            }
            line = new_line;
        }
    }
    line[current_idx] = '\0';
    if (eof_flag) {
        line[current_idx] = EOF;
    }

    return line;
}
