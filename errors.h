//
// Created by itay on 1/31/25.
//

#ifndef ERRORS_H
#define ERRORS_H

#define MEM_ALOC_ERROR() printf("Error: memory allocation error.\n")
#define FILE_OPEN_ERROR() printf("Error: failed to open file.\n")
#define EXTRA_CHARS_MACRO_ERROR(line) printf("Error in line %d: extra chars after macro defenition.\n", line)
#define MACRO_NAME_RESERVED(line) printf("Error in line %d: macro cannot have a reserved name.\n", line)


enum errors {NORMAL, ERROR};

typedef enum errors error_code;

#endif //ERRORS_H
