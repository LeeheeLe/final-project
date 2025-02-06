//
// Created by itay on 1/31/25.
//

#include "preproccesor.h"
struct line{
    char* line;
    struct line *next_line;
};
struct macro_table {
    char *macro_name;
    struct line *first_line;
    struct macro_table *next_macro;
};

int mcrostart(const char *line){
  int i, j;
  for (i=0; isspace(*(line+i)) && i<strlen(line); i++) {}
  for (j=0; *(line+i+j) == MACRO_START[j] && i<strlen(line) && j<strlen(MACRO_START);j++) {}
  if (j==strlen(MACRO_START) && isspace(*(line+i+j))){
      return 1;
  }
  return 0;
}

int mcroend(const char *line, error_code *ecode) {
    int i, j;
    for (i=0; isspace(*(line+i)) && i<strlen(line); i++) {}
    for (j=0; *(line+i+j) == MACRO_END[j] && i<strlen(line) && j<strlen(MACRO_END);j++) {}
    for (i=i+j; isspace(*(line+i)) && i<strlen(line); i++) {}
    if (j==strlen(MACRO_END)){
        if (i == strlen(line)) {
            return 1;
        }
        *ecode = EXTRA_CHARS_MACRO;
        return 1;
    }
    return 0;
}

int isreserved(char *word){
    /*TODO: Implement table of instructions and check if reserved name*/
}

void insert_macro_name(const char *line,struct macro_table *curr_macro, error_code *ecode){
    int i, j;
    char *macro_name = malloc(strlen(line)*sizeof(char));
    for (i=0; isspace(*(line+i)) && i<strlen(line); i++){} /*ignore whitespace*/
    for (j=0; *(line+i+j) == MACRO_START[j] && i<strlen(line) && j<strlen(MACRO_START);j++){}
    for (i=i+j; isspace(*(line+i)) && i<strlen(line); i++){} /*ignore whitespace*/
    for (j=0; isprint(*(line+i)) && !isspace(*(line+i)) && i<strlen(line); i++){
        *(macro_name+j) = *(line+i);
        j++;
    }
    if (isreserved(macro_name)){
        *ecode = MACRO_NAME_RESERVED;
    }
    for (; isspace(*(line+i)) && i<strlen(line); i++){} /*ignore whitespace*/
    if (i == strlen(line) - 1){
        curr_macro->macro_name = macro_name;
        return;
    }
    *ecode = EXTRA_CHARS_MACRO;
}

int is_saved_macro(const char *line,const struct macro_table *head, error_code *ecode) {
    int i, j, k=0;
    struct macro_table curr = *head;
    do {
        for (i=0; isspace(*(line+i)) && i<strlen(line); i++){} /*ignore whitespace*/
        for (j=0; *(line+i+j) == curr.macro_name[j] && i<strlen(line) && j<strlen(curr.macro_name);j++){}
        for (i=i+j; isspace(*(line+i)) && i<strlen(line); i++){} /*ignore whitespace*/
        if (i == strlen(line) - 1) {
            return k;
        }
        k++;
    } while (curr.next_macro != NULL);
    return -1;
}


error_code preprocess(char *filename){
    int macro_idx;
    error_code ecode = NORMAL;
    struct line file;
    struct line currline;
    struct macro_table curr_macro, *head_macro;
    head_macro = &curr_macro;
    /*TODO: start file read loop here*/
    char *line = read_line();
    if ((macro_idx = is_saved_macro(line,head_macro, &ecode)) != -1){
        replace_with_macro_content(macro_idx, head_macro);
        continue;
    }
    if (mcrostart(line)){
        insert_macro_name(line, &curr_macro, &ecode);
        while(1){
            line = read_line();
            if (!mcroend(line, &ecode)){
                append_line_to_macro(line, &curr_macro);
            }
            else{
                break;
            }
        }
    }
    else {
        write_line(line);
    }
    /*TODO: end file read loop here*/
}