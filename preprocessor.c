#include <stdio.h>
#include "input.h"
#include "preprocessor.h"

void preprocess(const char *input_file, const char *output_file) {
    int macro_idx, line_number = -1;
    error_code ecode = NORMAL;
    struct macro_table *curr_macro, *head_macro;
    head_macro = malloc(sizeof(struct  macro_table));
    curr_macro = head_macro;
    if (curr_macro == NULL) {
        MEM_ALOC_ERROR();
        return ;
    }
    FILE *input = fopen(input_file, "r"); /*open input file in 'read' mode*/
    FILE *output = fopen(output_file, "w"); /*open output file in 'write' mode*/
    if (output == NULL || input == NULL) {
        FILE_OPEN_ERROR();
    }
    char *line = NULL; /*pointer to a buffer for storing each line read from the file*/
    while ((line = getLine(input)) != NULL) {
        line_number++;
        if ((macro_idx = is_saved_macro(line, head_macro, &ecode)) != -1) {
            /*find index of macro in line if line is a macro*/
            print_macro_contents_to_file(macro_idx, head_macro, output); /*?*/
            continue;
        }
        if (mcro_start(line)) {
            insert_macro_name(line, curr_macro, &ecode, line_number);
            while ((line = getLine(input)) != NULL) {
                line_number++;
                if (!mcro_end(line, &ecode, line_number)) {
                    append_line_to_macro(line, curr_macro);
                } else {
                    break;
                }
            }
        } else {
            fputs(line, output);
            fputc('\n', output);
        }
        free(line);
    }
    fclose(input);
    fclose(output);
    while (head_macro != NULL) {
      struct macro_table *temp = head_macro;
      head_macro = head_macro->next_macro;
      free(temp);
    }
}

void append_line_to_macro(char *line,struct macro_table *curr_macro) {
    if (curr_macro->first_line == NULL) {
        curr_macro->first_line = malloc(sizeof(struct line));
        if (curr_macro->first_line == NULL) MEM_ALOC_ERROR();
    }
    struct line *curr_line = curr_macro->first_line;
    while (curr_line->next_line != NULL) {
        curr_line = curr_line->next_line;
    }
    curr_line->next_line = malloc(sizeof(struct line));
    curr_line->line = line;
}

void print_macro_contents_to_file(const int macro_idx,struct macro_table *head_macro, FILE *output) {
    int idx;
    struct macro_table curr_macro = *head_macro;
    struct line curr_line;
    for (idx = 0; idx < macro_idx; idx++) {
        curr_macro = *curr_macro.next_macro;
    }
    curr_line = *curr_macro.first_line;
    while (&curr_line != NULL && curr_line.line != NULL) {
        fputs(curr_line.line, output);
        fputc('\n', output);
        curr_line = *curr_line.next_line;
    }
}

int mcro_start(const char *line) {
    int i, j;
    for (i = 0; isspace(*(line + i)) && i < strlen(line); i++) {
    }
    for (j = 0; *(line + i + j) == MACRO_START[j] && i < strlen(line) && j < strlen(MACRO_START); j++) {
    }
    if (j == strlen(MACRO_START) && isspace(*(line + i + j))) {
        return 1;
    }
    return 0;
}

int mcro_end(const char *line, error_code *ecode, const int line_number) {
    int i, j;
    for (i = 0; isspace(*(line + i)) && i < strlen(line); i++) {
    }
    for (j = 0; *(line + i + j) == MACRO_END[j] && i < strlen(line) && j < strlen(MACRO_END); j++) {
    }
    for (i = i + j; isspace(*(line + i)) && i < strlen(line); i++) {
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

char reserved_names[28] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
    "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne",
    "jsr", "red", "prn", "rts", "stop", ".data", ".string",".entry", ".extern"};

int is_reserved_name(char *mcro_name){
    int i;
    for(i = 0 ; i < sizeof(reserved_names)/sizeof(reserved_names[0]) ; i++){
        if(strcmp(&reserved_names[i],mcro_name) == 0){
        /*name is already reserved*/
        return 1;
        }
    }
    return 0; /*valid macro name*/
}

void insert_macro_name(const char *line, struct macro_table *curr_macro, error_code *ecode, int line_number) {
    int i, j;
    char *macro_name = malloc(strlen(line) * sizeof(char));
    for (i = 0; isspace(*(line + i)) && i < strlen(line); i++) {
    } /*ignore whitespace*/
    for (j = 0; *(line + i + j) == MACRO_START[j] && i < strlen(line) && j < strlen(MACRO_START); j++) {
    }
    for (i = i + j; isspace(*(line + i)) && i < strlen(line); i++) {
    } /*ignore whitespace*/
    for (j = 0; isprint(*(line + i)) && !isspace(*(line + i)) && i < strlen(line); i++) {
        *(macro_name + j) = *(line + i);
        j++;
    }
    if ( is_reserved_name(macro_name)) {
      *ecode = MACRO_NAME_RESERVED(line_number);
    }
    for (i=0 ; isspace(*(line + i)) && i < strlen(line); i++) {
    } /*ignore whitespace*/
    if (i == strlen(line)) {
        curr_macro->macro_name = macro_name;
        return;
    }
    *ecode = ERROR;
    EXTRA_CHARS_MACRO_ERROR(line_number);
}

int is_saved_macro(const char *line, struct macro_table *head, error_code *ecode) {
    int i, j, k = 0;
    struct macro_table curr = *head;
    while (&curr != NULL && curr.macro_name != NULL) {
        for (i = 0; isspace(*(line + i)) && i < strlen(line); i++) {
        } /*ignore whitespace*/
        for (j = 0; *(line + i + j) == curr.macro_name[j] && i < strlen(line) && j < strlen(curr.macro_name); j++) {
        }
        for (i = i + j; isspace(*(line + i)) && i < strlen(line); i++) {
        } /*ignore whitespace*/
        if (i == strlen(line)) {
            return k;
        }
        k++;
        if (curr.next_macro == NULL) {
            return -1;
        }
        curr = *curr.next_macro;
    }
    return -1;
}