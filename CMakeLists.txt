cmake_minimum_required(VERSION 3.30.5)
project(final_project C)

set(CMAKE_C_STANDARD 90)

include_directories(.)

add_executable(final_project
        "Header files/errors.h"
        "source files/input.c"
        "Header files/input.h"
        "source files/main.c"
        "source files/preprocessor.c"
        "Header files/preprocessor.h"
        "source files/first_pass.c"
        "Header files/first_pass.h"
        "Header files/file_extensions.h"
        "source files/label_table.c"
        "source files/parsing.c"
        "Header files/parsing.h"
        "source files/second_pass.c"
        "source files/const_tables.c"
        "source files/intern_table.c"
        "Header files/tables.h"
        "Header files/mem_image.h")
