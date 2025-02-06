//
// Created by itay on 1/31/25.
//

#ifndef ERRORS_H
#define ERRORS_H

enum errors {NORMAL,
    EXTRA_CHARS_MACRO,
    MACRO_NAME_RESERVED};

typedef enum errors error_code;

#endif //ERRORS_H
