//
// Created by itay on 1/31/25.
//

#include "preproccesor.h"

int main(int argc, char *argv[]){
    int i;
    char *filename;
    for (i=1; i<argc; i++){
        filename = argv[i];
        preprocess(filename);
        firstpass(filename);
        secondpass(filename);

    }
}