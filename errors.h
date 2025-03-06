#ifndef ERRORS_H
#define ERRORS_H

#define MEM_ALOC_ERROR() printf("Error: memory allocation error.\n")
#define FILE_OPEN_ERROR() printf("Error: failed to open file.\n")
#define EXTRA_CHARS_MACRO_ERROR(line) printf("Error in line %d: extra chars after macro defenition.\n", line)
#define MACRO_NAME_RESERVED(line) printf("Error in line %d: macro cannot have a reserved name.\n", line)
#define INVALID_INSTRUCTION(line) printf("Error in line %d: invalid instruction name.\n", line)

#define LABELED_LINKING_WARNING(line) printf("Warning in line %d: labeling a .extern or a .entry instruction has no meaning.\n", line)

enum errors {NORMAL, ERROR};

typedef enum errors error_code;

#endif /*ERRORS_H*/