#include <stdio.h>
#include "../Header files/preprocessor.h"
#include "../Header files/first_pass.h"

int main(const int argc, char *argv[]) {

    if (argc < 2) {
        printf("error: missing argument\n");
        return 1;
    }
    int i;
    for (i = 1; i < argc; i++) {
        preprocess(argv[i]);
        first_pass(argv[i]);
    }
    return 0;
}