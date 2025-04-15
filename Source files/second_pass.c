#include "../Header files/second_pass.h"
#include "../Header files/memory_utility.h"


void create_ob_file(const char *file_name, const memory *code,
                    const memory *data, int ICF, const int DCF) {
  char *file_ob_name = add_extension(file_name, OBJECT_FILE_EXTENTION);
  FILE *file_ob = fopen(file_ob_name, "w");
  free_ptr(file_ob_name);
  int i = 0, j = 0;

  if (file_ob == NULL) {
    FILE_OPEN_ERROR();
    free_all_memory();
    exit(EXIT_FAILURE);
  }

  fprintf(file_ob, "  %d %d\n", ICF-START_ADDRESS, DCF); /* header */

  for (i = 0; i + START_ADDRESS < ICF; i++) { /* machine code */
    fprintf(file_ob, "%07d %06x\n", i + START_ADDRESS, (*code)[i+START_ADDRESS].data.value);
  }
  for (j = 0; j < DCF; j++) {
    fprintf(file_ob, "%07d %06x\n", j + ICF, (*data)[j].data.value);
  }
  fclose(file_ob);
}

void create_entry_file(const char *file_name,
                       const label_table_head label_table,
                       const entry_table_head entry_table, const int ICF) {
  char *file_ent_name = add_extension(file_name, ENTRIES_FILE_EXTENTION);
  FILE *file_entry = fopen(file_ent_name, "w");
  free_ptr(file_ent_name);

  if (file_entry == NULL) {
    FILE_OPEN_ERROR();
    free_all_memory();
    exit(EXIT_FAILURE);
  }

  entry_node *current;
  label_node *found_label;
  current = entry_table.root;

  while (current != NULL) {
    if ((found_label = find_label(current->name, label_table)) == NULL) {
      //todo: throw error entry name not defined in the code
    } else {
      if (found_label->value == DEFAULT_EXTERN_VALUE) {
        //todo: throw error extern cannot be intern too
      }
      fprintf(file_entry, "%s %07d\n", current->name, (found_label->type == DATA) ? found_label->value + ICF : found_label->value);
    }
    current = current->next_entry;
  }
  fclose(file_entry);
}

void populate_labels(const char *file_name, memory *code,
                     const label_table_head label_table,
                     const intern_table_head intern_table, const int ICF) {
  char *file_ext_name = add_extension(file_name, EXTERNALS_FILE_EXTENTION);
  FILE *file_extern = fopen(file_ext_name, "w");
  free_ptr(file_ext_name);

  if (file_extern == NULL) {
    FILE_OPEN_ERROR();
    free_all_memory();
    exit(EXIT_FAILURE);
  }

  intern_node *current = intern_table.root;
  label_node *found_label;
  while (current != NULL) {
    if ((found_label = find_label(current->name, label_table)) == NULL) {
      //todo: throw error label used but not declared
    } else {
      if (current->type == immediate) {
        int value = found_label->value;
        value = (found_label->type==DATA) ? value + ICF : value;
        (*code)[current->mem_place].operand.value = value;
        if (found_label->linking_type == EXTERN) {
          (*code)[current->mem_place].operand.E = 1;
          (*code)[current->mem_place].operand.value = 0;
          fprintf(file_extern, "%s %7d\n", current->name, current->mem_place);
        } else {
          (*code)[current->mem_place].operand.R = 1;
        }
      } else if (current->type == relative) {
        if (found_label->linking_type == EXTERN) {
          //todo: throw error cannot use extern label as relative param
        } else {
          (*code)[current->mem_place].operand.A = 1;
          (*code)[current->mem_place].operand.value = found_label->value-current->mem_place+1;
        }
      }
    }
    current = current->next_intern;
  }
  fclose(file_extern);

}