#include <second_pass.h>
#include <memory_utility.h>

/*
 * create_ob_file - Generates the object file that contains both machine code and data.
 * @file_name: The name of the source file to generate the object file for.
 * @code: The machine code memory array.
 * @data: The data memory array.
 * @ICF: The final value of the instruction counter.
 * @DCF: The final value of the data counter.
 *
 * This function generates an object file (.ob) containing the machine code and data.
 * It writes the ICF (instruction counter) and DCF (data counter) in the header and
 * follows with the corresponding machine code and data.
 * The machine code starts at the address specified by the constant `START_ADDRESS`.
 * Each entry in the file consists of an address and a value in hexadecimal format.
 */
void create_ob_file(const char *file_name, const memory code,
                    const memory data, int ICF, const int DCF) {
  int i = 0, j = 0;
  char *file_ob_name = add_extension(file_name, OBJECT_FILE_EXT);
  FILE *file_ob = fopen(file_ob_name, "w");
  free_ptr(file_ob_name);

  if (file_ob == NULL) {
    FILE_OPEN_ERROR();
    free_all_memory();
    exit(EXIT_FAILURE);
  }

  fprintf(file_ob, "  %d %d\n", ICF - START_ADDRESS, DCF); /* header */

  /* Write machine code*/
  for (i = 0; i + START_ADDRESS < ICF; i++) {
    fprintf(file_ob, "%07d %06x\n", i + START_ADDRESS, (code)[i + START_ADDRESS].data.value);
  }

  /* Write data*/
  for (j = 0; j < DCF; j++) {
    fprintf(file_ob, "%07d %06x\n", j + ICF, (data)[j].data.value);
  }

  fclose(file_ob);
}

/*
 * create_entry_file - Generates the entry file containing all the entry labels.
 * @file_name: The name of the source file to generate the entry file for.
 * @label_table: The label table that contains all label information.
 * @entry_table: The entry table that contains all entry label names.
 * @ICF: The final instruction counter value.
 *
 * This function generates an entry file (.ent) listing the names and addresses
 * of the entry labels. Each entry consists of a label name and its address in the code
 * or data section. If any entry label is not found in the label table or is marked as
 * EXTERN, an error will be triggered.
 * If there are no entry labels, the entry file is removed.
 */
void create_entry_file(const char *file_name,
                       const label_table_head label_table,
                       const entry_table_head entry_table, const int ICF) {
  int remove_entry_file = 1;
  entry_node *current;
  label_node *found_label;
  char *file_ent_name = add_extension(file_name, ENTRIES_FILE_EXT);
  FILE *file_entry = fopen(file_ent_name, "w");

  if (file_entry == NULL) {
    FILE_OPEN_ERROR();
    free_all_memory();
    exit(EXIT_FAILURE);
  }

  current = entry_table.root;

  while (current != NULL) {
    if ((found_label = find_label(current->name, label_table)) == NULL) {
      /* Handle error if entry name is not defined in the code */
    } else {
      if (found_label->value == DEFAULT_EXTERN_VALUE) {
        /* Handle error if entry is marked as EXTERN */
      }
      remove_entry_file = 0;
      fprintf(file_entry, "%s %07d\n", current->name, (found_label->type == DATA) ? found_label->value + ICF : found_label->value);
    }
    current = current->next_entry;
  }
  fclose(file_entry);

  /* Remove entry file if no entries were written*/
  if (remove_entry_file) {
    remove(file_ent_name);
  }
}

/*
 * populate_labels - Populates labels in code, modifying the code memory values accordingly.
 * @file_name: The name of the source file.
 * @code: The machine code memory array.
 * @label_table: The label table that contains all label information.
 * @intern_table: The intern table that contains all internal labels.
 * @ICF: The instruction counter final value.
 *
 * This function updates the machine code array by replacing label references with
 * their corresponding values. It also generates an extern file (.ext) for labels with external references.
 * The labels are checked for validity, and any errors encountered (e.g., undefined labels or invalid label types) are flagged.
 * The extern file is removed if no extern labels are present.
 */
void populate_labels(const char *file_name, memory code,
                     const label_table_head label_table,
                     const intern_table_head intern_table, const int ICF) {
  int remove_extern_file = 1;
  intern_node *current = intern_table.root;
  label_node *found_label;
  char *file_ext_name = add_extension(file_name, EXTERNALS_FILE_EXT);
  FILE *file_extern = fopen(file_ext_name, "w");

  if (file_extern == NULL) {
    FILE_OPEN_ERROR();
    free_all_memory();
    exit(EXIT_FAILURE);
  }

  while (current != NULL) {
    if ((found_label = find_label(current->name, label_table)) == NULL) {
      /* Handle error if label used but not declared */
    } else {
      if (current->type == immediate) {
        int value = found_label->value;
        value = (found_label->type == DATA) ? value + ICF : value;
        (code)[current->mem_place].operand.value = value;

        if (found_label->linking_type == EXTERN) {
          (code)[current->mem_place].operand.E = 1;
          (code)[current->mem_place].operand.value = 0;
          remove_extern_file = 0;
          fprintf(file_extern, "%s %7d\n", current->name, current->mem_place);
        } else {
          (code)[current->mem_place].operand.R = 1;
        }
      } else if (current->type == relative) {
        if (found_label->linking_type == EXTERN) {
          /* Handle error: cannot use extern label as relative parameter */
        } else {
          (code)[current->mem_place].operand.A = 1;
          (code)[current->mem_place].operand.value = found_label->value - current->mem_place + 1;
        }
      }
    }
    current = current->next_intern;
  }

  fclose(file_extern);

  /*Remove extern file if no extern labels were written*/
  if (remove_extern_file) {
    remove(file_ext_name);
  }
}