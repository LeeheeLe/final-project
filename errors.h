#ifndef ERRORS_H
#define ERRORS_H

/*Memory error macros*/
#define MEM_ALOC_ERROR() printf("Error: memory allocation error.\n")
#define FILE_OPEN_ERROR() printf("Error: failed to open file.\n")

/*macro errors*/
#define EXTRA_CHARS_MACRO_ERROR(line) printf("Error in line %d: extra chars after macro defenition.\n", line)
#define MACRO_NAME_RESERVED(line) printf("Error in line %d: macro cannot have a reserved name.\n", line)

/*parsing errors*/
#define INVALID_INSTRUCTION(line) printf("Error in line %d: invalid instruction name.\n", line)
#define CONFLICTING_LABELS(line, label_name) printf("Error in line %d: label %s already exists.\n", line, label_name)
#define MISSING_INSTRUCTION_PARAM(line) printf("Error in line %d: missing instruction parameter.\n", line)
#define MISSING_STRING_INDICATOR(line) printf("Error in line %d: missing string.\n", line)
#define EXTRA_CHARS_STRING_ERROR(line) printf("Error in line %d: extra chars after string instruction \n", line)
#define EXTRA_CHARS_EXTERN_ERROR(line) printf("Error in line %d: extra chars after extern instruction \n", line)
#define LABEL_TOO_LONG(line) printf("Error in line %d: label too long.\n", line)
#define MISSING_COMMA(line) printf("Error in line %d: missing comma.\n", line)
#define MISSING_NUMBER_OR_EXTRA_COMMA(line) printf("Error in line %d: missing number or extraneous comma.\n", line)
#define NON_EXISTANT_NAME(name) printf("The operation named %s does not exist.\n", name);

/*warnings*/
#define LABELED_LINKING_WARNING(line) printf("Warning in line %d: labeling a .extern or a .entry instruction has no meaning.\n", line)

enum errors {NORMAL, ERROR};


#endif /*ERRORS_H*/