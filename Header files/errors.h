#ifndef ERRORS_H
#define ERRORS_H

/*
 * File: errors.h
 * --------------
 * This header contains all error and warning macros used throughout
 * the assembler. These macros standardize the error and warning messages
 * printed during assembly parsing and validation, making debugging and
 * user feedback consistent.
 *
 * Categories:
 *  - Memory errors
 *  - Macro definition errors
 *  - Parsing and syntax errors
 *  - Warnings for non-critical issues
 */

/*
 * Memory error macros
 * These are printed when critical memory or file access issues occur.
 */
#define MEM_ALOC_ERROR() printf("Error: memory allocation error.\n")
#define FILE_OPEN_ERROR() printf("Error: failed to open file.\n")

/*
 * Macro-related errors
 * Used when processing macro definitions.
 */
#define EXTRA_CHARS_MACRO_ERROR(line) printf("Error in line %d: extra chars after macro defenition.\n", line)
#define MACRO_NAME_RESERVED(line) printf("Error in line %d: macro cannot have a reserved name.\n", line)

/*
 * Parsing and instruction errors
 * Used to catch syntax or logic issues during line parsing.
 */
#define INVALID_INSTRUCTION(line) printf("Error in line %d: invalid instruction name.\n", line)
#define CONFLICTING_LABELS(line, label_name) printf("Error in line %d: label %s already exists.\n", line, label_name)
#define LABEL_MACRO_CONFLICT(line, label_name) printf("Error in line %d: label %s already defined as macro.\n", line, label_name)
#define MISSING_INSTRUCTION_PARAM(line) printf("Error in line %d: missing instruction parameter.\n", line)
#define MISSING_STRING_INDICATOR(line) printf("Error in line %d: missing string.\n", line)
#define EXTRA_CHARS_STRING_ERROR(line) printf("Error in line %d: extra chars after string instruction \n", line)
#define EXTRA_CHARS_LINKING_ERROR(line) printf("Error in line %d: extra chars after extern or entry instruction \n", line)
#define LABEL_TOO_LONG(line) printf("Error in line %d: label too long.\n", line)
#define MISSING_COMMA(line) printf("Error in line %d: missing comma.\n", line)
#define MISSING_NUMBER_OR_EXTRA_COMMA(line) printf("Error in line %d: missing number or extraneous comma.\n", line)
#define NON_EXISTANT_NAME(name) printf("The operation named %s does not exist.\n", name)
#define MISSING_OPERAND(line) printf("Error in line %d: missing operand.\n", line)
#define FILE_EXTENSION_ERROR(file_name) printf("Error in file %d: file names entered should not include the extention.\n", file_name)

/*
 * Warnings
 * Used for notifying the user of non-critical issues that may indicate
 * misuse or unnecessary syntax.
 */
#define LABELED_LINKING_WARNING(line) printf("Warning in line %d: labeling a .extern or a .entry instruction has no meaning.\n", line)

/*
 * Enum: errors
 * Represents a basic error status indicator.
 *
 * Values:
 *  NORMAL - No error occurred
 *  ERROR  - An error has occurred
 */
enum errors {NORMAL, ERROR};

#endif /* ERRORS_H */
