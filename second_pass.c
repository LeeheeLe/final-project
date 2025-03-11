#include "second_pass.h"
#include <stdio.h>
#include "input.h"
#include <string.h>
#include "errors.h"

/*algorithem:
* 1. read next line from file. if end of file go to 7
* 2. if the first field in the row is a label, skip it
* 3. if is it .data, .string or .extern, go to 1
* 4. is it .entry? if not, go to 6
* 5. add 'entry' in label table to the to the label (if label is not in the table, error)
* 6. complete the binary coding of the operands,  בהתאם לשיטות מיעון שבשימוש.
*      for every operand that has a label, find it's value in the label table (error if not in the table)
*      if label is marked as external, add the adress of מילת המידע הרלוונטית לרשימת מילות מידע שמתייחסות לסמל חיצוני.
*      go back to 1
* 7. we have read the entire source code. if found errors stop here
* 8. build output files*/

struct second_pass *second_pass(const char *file_name) {
    char *input_file, *output_file;
    int line_number = 1;
    int is_extern_flag = 0;
    /*input_file = malloc(strlen(file_name) + strlen(INPUT_EXT) + 1);
    output_file = malloc(strlen(file_name) + strlen(OUTPUT_EXT) + 1);
    if (input_file == NULL || output_file == NULL) {
        MEM_ALOC_ERROR();
        return NULL;
    }
    input_file = strcpy(input_file, file_name);
    output_file = strcpy(output_file, file_name);
    strcat(input_file, INPUT_EXT);
    strcat(output_file, OUTPUT_EXT); */

    FILE *input = fopen(input_file, "r"); /*open input file in 'read' mode*/
    FILE *output = fopen(output_file, "w"); /*open output file in 'write' mode*/
    /*free(input_file);
    free(output_file);*/
    if (output == NULL || input == NULL) {
        FILE_OPEN_ERROR();
        return NULL;
    }
    char *line = NULL; /*pointer to a buffer for storing each line read from the file*/
    while ((line = getLine(input)) != NULL) { /*while not EOF*/
        line_number++;
        if (is_label(line)) {
          free(line);
          continue;
        }
        if (is_data(line) || is_extern(line) || is_string(line)) {
          if (is_extern(line) == 1) { is_extern_flag = 1; }
          free(line);
          continue;
        }
       if (is_entry(line)) {
         add_entry_to_label_table(line); /*if label not in the table, error in the implementation of func*/
         free(line);
       } else{ /*stage 6: complete the binary coding of the operands*/
         find_value_in_label_table(line); /*return error if not in the table*/
       }
       if (is_extern_flag == 1) {
         add_extern_adress(line);
       }
    }
    /* finished reading the file. go to 7: if found errors stop here */

    fclose(input);
    fclose(output);

}
 build_output_files(input_file, output_file){

}

/* TODO: implement is_label() add_entry_to_label_table... */