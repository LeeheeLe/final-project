#include "preproccesor.h"
#include "input.h"
#include <stdio.h>


/*TODO */
void preprocces(const char *input_file, const char *output_file) {
    int macro_idx, line_number = -1;
    error_code ecode = NORMAL;
    struct line file;
    struct line currline;
    struct macro_table curr_macro, *head_macro;
    head_macro = &curr_macro;

    FILE *output = fopen(output_file, "w"); /*open output file in 'write' mode*/
    if (output == NULL) {
        FILE_OPEN_ERROR();
    }

    char *line = NULL; /*pointer to a buffer for storing each line read from the file*/
    size_t buffer_length = 0; /*track the buffer size*/
    size_t number_of_read_chars; /*numbers of chars read*/


    while ((line = getLine(input_file)) != NULL) {
        line_number++;
        if ((macro_idx = is_saved_macro(line, head_macro, &ecode)) != -1) {
            /*find index of macro in line if line is a macro*/
            replace_with_macro_content(macro_idx, head_macro); /*?*/
            continue;
        }
        if (mcrostart(line)) {
            insert_macro_name(line, &curr_macro, &ecode, line_number);
            while ((line = getLine(input_file)) != NULL) {
                line_number++;
                if (!mcroend(line, &ecode)) {
                    append_line_to_macro(line, &curr_macro);
                } else {
                    break;
                }
            }
        } else {
            fputs(line, output);
        }
        free(line);
        fclose(input);
        fclose(output);
        printf("\n");
    }


    int mcrostart(const char *line) {
        int i, j;
        for (i = 0; isspace(*(line+i)) && i < strlen(line); i++) {
        }
        for (j = 0; *(line + i + j) == MACRO_START[j] && i < strlen(line) && j < strlen(MACRO_START); j++) {
        }
        if (j == strlen(MACRO_START) && isspace(*(line+i+j))) {
            return 1;
        }
        return 0;
    }

    int mcroend(const char *line, error_code *ecode, int line_number) {
        int i, j;
        for (i = 0; isspace(*(line+i)) && i < strlen(line); i++) {
        }
        for (j = 0; *(line + i + j) == MACRO_END[j] && i < strlen(line) && j < strlen(MACRO_END); j++) {
        }
        for (i = i + j; isspace(*(line+i)) && i < strlen(line); i++) {
        }
        if (j == strlen(MACRO_END)) {
            if (i == strlen(line)) {
                return 1;
            }
            *ecode = ERROR;
            EXTRA_CHARS_MACRO_ERROR(line_number);
            return 1;
        }
        return 0;
    }

    int isreserved(char *word) {
        /*TODO: Implement table of instructions and check if reserved name*/
    }

    void insert_macro_name(const char *line, struct macro_table *curr_macro, error_code *ecode, int line_number) {
        int i, j;
        char *macro_name = malloc(strlen(line) * sizeof(char));
        for (i = 0; isspace(*(line+i)) && i < strlen(line); i++) {
        } /*ignore whitespace*/
        for (j = 0; *(line + i + j) == MACRO_START[j] && i < strlen(line) && j < strlen(MACRO_START); j++) {
        }
        for (i = i + j; isspace(*(line+i)) && i < strlen(line); i++) {
        } /*ignore whitespace*/
        for (j = 0; isprint(*(line+i)) && !isspace(*(line+i)) && i < strlen(line); i++) {
            *(macro_name + j) = *(line + i);
            j++;
        }
        if (isreserved(macro_name)) {
            MACRO_NAME_RESERVED(line_number);
        }
        for (; isspace(*(line+i)) && i < strlen(line); i++) {
        } /*ignore whitespace*/
        if (i == strlen(line) - 1) {
            curr_macro->macro_name = macro_name;
            return;
        }
        *ecode = ERROR;
        EXTRA_CHARS_MACRO_ERROR(line_number)
    }

    int is_saved_macro(const char *line, struct macro_table *head, error_code *ecode) {
        int i, j, k = 0;
        struct macro_table curr = *head;
        do {
            for (i = 0; isspace(*(line+i)) && i < strlen(line); i++) {
            } /*ignore whitespace*/
            for (j = 0; *(line + i + j) == curr.macro_name[j] && i < strlen(line) && j < strlen(curr.macro_name); j++) {
            }
            for (i = i + j; isspace(*(line+i)) && i < strlen(line); i++) {
            } /*ignore whitespace*/
            if (i == strlen(line) - 1) {
                return k;
            }
            k++;
        } while (curr.next_macro != NULL);
        return -1;
    }
