#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "errors.h"
#include "file_extensions.h"
#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ASSEMBLER_INPUT_EXT PREPROCESSOR_OUTPUT_EXT
/*the input of the first pass is the output of the preprocessor*/

void first_pass(const char *file_name);

#endif // FIRST_PASS_H