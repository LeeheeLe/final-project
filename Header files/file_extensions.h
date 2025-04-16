#ifndef FILE_EXTENSIONS_H
#define FILE_EXTENSIONS_H

/*
 * File: file_extensions.h
 * -----------------------
 * This header defines constants for the various file extensions used
 * throughout the assembler process. These macros ensure consistent
 * naming of input, intermediate, and output files during all stages
 * of assembly.
 *
 * Extensions are used for:
 *  - Preprocessor input and output
 *  - Final object code
 *  - External references
 *  - Entry point labels
 */

/*
 * Preprocessor file extensions
 * Used before and after macro expansion.
 */
#define PREPROCESSOR_INPUT_EXT ".as"  /* Original source file */
#define PREPROCESSOR_OUTPUT_EXT ".am" /* After macro processing */

/*
 * Assembler output file extensions
 */
#define OBJECT_FILE_EXT ".ob"    /* Final assembled machine code */
#define EXTERNALS_FILE_EXT ".ext" /* External labels used by this file */
#define ENTRIES_FILE_EXT ".ent"  /* Entry labels defined in this file */

#endif /* FILE_EXTENSIONS_H */