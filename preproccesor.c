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

error_code preprocces(char *filename){
    int mcro_flag = 0;
    struct line file;
    struct line currline;
    struct macro_table curr_macro;
    char *line = read_line();
    if (starts_with_macro()){
        replace_with_macro_content();
        continue;
    }
    if (def_start()){
        insert_macro_name(line, &curr_macro);
        rmline();
        while(1){
            line = read_line();
            if (!mcroend(line)){
                append_line_to_macro(line, &curr_macro);
            }
            else{
                mcro_flag = 0;
                continue;
            }
        }
    }
    write_line(line);


}