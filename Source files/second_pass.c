#include <string.h>
#include "../Header files/second_pass.h"
#include "../Header files/errors.h"
#include "../Header files/input.h"
#include "../Header files/label_table.h"
#include <stdio.h>
#include "../Header files/parsing.h"

char *add_extension(char *filename, char *extension) {
    char *new_filename;
    long filename_length = strlen(filename);
    long extension_length = strlen(extension);

    if (extension_length < filename_length) { /* if the file name already has an extention */
        if (strcmp(filename + filename_length - extension_length, extension) == 0) {
            FILE_EXTENSION_ERROR(filename);
            return NULL;
        }
    }
    new_filename = (char *)malloc(sizeof(filename_length + extension_length + 1));
    if (new_filename == NULL)  /*memory allocation failed */
		MEM_ALOC_ERROR();
        exit(1);

    strcpy(new_filename, filename);
    strcat(new_filename, extension);
    return new_filename;
}

char *change_extension(char *file_name, char *new_extension); //TODO; implement

void create_ob_file(char *file_ob_name, unsigned short *code, unsigned short *data, int *IC, int *DC) {
    FILE *file_ob = fopen(file_ob_name,"w");
    int i = 0, j = 0;

    if (file_ob == NULL) {
        FILE_OPEN_ERROR();
        free_all_memory(); //TODO: implement
        exit(1);
    }

    fprintf(file_ob,"  %d %d\n",*IC,*DC);  /* header */

    for (i ; i< *IC; i++) {    /* machine code */
        fprintf(file_ob,"%07d %06x\n",i+START_ADDRESS,code[i]);
    }
    for (j ; j< *DC; j++) {
        fprintf(file_ob,"%07d %06x\n",j+i+START_ADDRESS,data[j]);
    }

    fclose(file_ob);
}

void create_entry_file(char *file_entry_name) {
    FILE *file_entry = fopen(file_ent_name,"w");
    label *current;

    if (file_entry == NULL) {
        FILE_OPEN_ERROR();
        free_all_memory(); //TODO: implement
        exit(1);
    }

    current = table_head;
    while (current != NULL) {
        if (current->label_data_type == ENTRY) {
            fprintf(file_entry,"%s %07d\n", /*lable name and label adress)*/;
        }
        current = current->next; //I implemented like linked list and not like the binary tree,
                                 // TODO: fix and combine with interns
    }
    fclose(file_ent);
}

void create_ext_file(char *file_ext_name) {
    FILE *file_ext = fopen(file_ext_name,"w");
    label *current;

    if (file_ext == NULL) {
        FILE_OPEN_ERROR();
        free_all_memory();
        exit(1);
    }

    current = label_head();
    while (current != NULL) {
        if (current->type == EXTERN && current->adress == CODE) {
            fprintf(file_ext,"%s %06d\n",current->name,current->address);
        }
        current = current->next;
    }
    fclose(file_ext);
}


/*algorithm:
 * 1. read next line from file. if end of file go to 7
 * 2. if the first field in the row is a label, skip it
 * 3. if is it .data, .string or .extern, go to 1
 * 4. is it .entry? if not, go to 6
 * 5. add 'entry' in label table to the to the label (if label is not in the
 * table, error)
 * 6. complete the binary coding of the operands,  בהתאם לשיטות מיעון שבשימוש.
 *      for every operand that has a label, find it's value in the label table
 * (error if not in the table) if label is marked as external, add the address of
 * מילת המידע הרלוונטית לרשימת מילות מידע שמתייחסות לסמל חיצוני. go back to 1
 * 7. we have read the entire source code. if found errors stop here
 * 8. build output files*/

/*
struct second_pass *second_pass(const char *file_name) {
  char *input_file;
  int line_number = 1;
  input_file = malloc(strlen(file_name) + strlen(INPUT_EXT) + 1);
  if (input_file == NULL) {
    MEM_ALOC_ERROR();
    return NULL;
  }
  input_file = strcpy(input_file, file_name);
  strcat(input_file, INPUT_EXT);
  FILE *input = fopen(input_file, "r"); /*open input file in 'read' mode#1#
  free(input_file);
  if (input == NULL) {
    FILE_OPEN_ERROR();
    return NULL;
  }
  char *work_line, *line, *label_name;
  inst instruction_type;
  while ((work_line = line = getLine(input)) != NULL) { /*while not EOF#1#
    line_number++;
    if (is_label(&work_line, &label_name)) {
      free(line);
      continue;
    }
    if (is_instruction(&work_line, &instruction_type, line_number)) {
      if (instruction_type == ENTRY_INST) {
        add_entry_to_label_table(line); /*if label not in the table, error in
                                           the implementation of func#1#
        free(line);
      }
      free(line);
      continue;
    }
    /*stage 6: complete the binary coding of the operands#1#
    find_value_in_label_table(line); /*return error if not in the table#1#
  }
  if (is_entry_flag == 1) {
    add_extern_adress(line);
  }
}
  /* finished reading the file. go to 7: if found errors stop here #1#

  fclose(input);
  fclose(output);
}
build_output_files(input_file, output_file) {}
*/
