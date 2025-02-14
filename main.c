#include <stdio.h>
#include "preproccesor.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("error: missing argument\n");
        return 1;
    }
    int i;
    for (i = 1; i < argc; i++) {
        size_t output_len = strlen(argv[i]) + strlen(PREPROCESSED_EXT) + 1;
        char *input_file = (char *)malloc(output_len);
        char *output_file = (char *)malloc(output_len);
        if (output_file == NULL || input_file == NULL) {
            MEM_ALOC_ERROR();
            return 1;
        }
        strcat(output_file, argv[i]);
        strcat(output_file, PREPROCESSED_EXT);
        strcat(input_file, argv[i]);
        strcat(input_file, INPUT_EXT);
        preprocess(input_file, output_file);
        free(input_file);
        free(output_file);
    }
    return 0;
}
