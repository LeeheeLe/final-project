cmake_minimum_required(VERSION 3.31)
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
        first_pass.h)
