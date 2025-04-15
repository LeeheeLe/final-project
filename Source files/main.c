#include "../Header files/first_pass.h"
#include "../Header files/preprocessor.h"
#include "../Header Files/memory_utility.h"
#include <stdio.h>

/*
 * Purpose:
 * The entry point of the program. This function processes the command-line
 * arguments, performs preprocessing on each file, and then runs the first pass
 * assembler process on the specified files.
 *
 * The program expects at least one argument (the file to process). If no
 * arguments are provided, an error message is displayed.
 *
 * Functionality:
 * - Processes each file specified in the command line arguments.
 * - Calls the `preprocess` function on each file to handle preprocessing.
 * - Calls the `first_pass` function to execute the first pass of the assembler
 * on each file.
 * //todo complete this documenting acording to the last things we finish
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line argument strings.
 *
 * @return int Returns 0 if successful, 1 if an error occurs (like missing file
 * argument).
 */
int main(const int argc, char *argv[]) {

  /* Check if there are fewer than 2 command-line arguments */
  if (argc < 2) {
    printf("error: missing argument\n"); /* Display error if no file is given */
    return 1;                            /* Return error code */
  }

  int i;
  /* Loop through each command-line argument (file to process) */
  for (i = 1; i < argc; i++) {
    struct Macro_table *macros = preprocess(argv[i]); /* Call the preprocess function on the file */
    if (macros == NULL){
      continue;
    }
    first_pass(argv[i], macros); /* Call the first pass assembler on the file */
    free_all_memory();
  }
  return 0; /* Return success code */
}
