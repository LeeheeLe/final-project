#include <stdio.h>
#include "preproccesor.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("error: missing argument\n");
        /*TODO: make error enum for error id*/
        return 1;
    }
    int i;
    for (i = 1; i < argc; i++) {
        size_t output_len = strlen(argv[i]) + strlen("_preprocessed.asm") + 1;
        char *output_file = (char *)malloc(output_len);
        if (output_file == NULL) {
            perror("Memory allocation error");
            return 1;
        }
        snprintf(output_file, output_len, "%s_preprocessed.asm", argv[i]);
        preprocces(argv[i], output_file);
        free(output_file);
    }
    return 0;
}
