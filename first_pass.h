#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "errors.h"
#include "file_extensions.h"
#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LABEL_DEF_CHAR ':'
#define INPUT_EXT PREPROCESSOR_OUTPUT_EXT
/*the input of the first pass is the output of the preprocessor*/

void first_pass(const char *file_name);
int is_label(char **line, char **label_name);

#endif // FIRST_PASS_H
