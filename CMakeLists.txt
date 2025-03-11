cmake_minimum_required(VERSION 3.30.5)
project(final_project C)

set(CMAKE_C_STANDARD 90)

include_directories(.)

add_executable(final_project
        errors.h
        input.c
        input.h
        main.c
        preprocessor.c
        preprocessor.h
        first_pass.c
        first_pass.h
        file_extensions.h
        label_table.c
        label_table.h
        parsing.c
        parsing.h
        mem_image.c
        mem_image.h)
