cmake_minimum_required(VERSION 3.30.5)
project(final_project C)

set(CMAKE_C_STANDARD 90)

include_directories(.)

add_executable(final_project
        "Source files/entry_table.c"
        "Source files/const_tables.c"
        "Source files/first_pass.c"
        "Source files/input.c"
        "Source files/intern_table.c"
        "Source files/label_table.c"
        "Source files/main.c"
        "Source files/parsing.c"
        "Source files/preprocessor.c"
        "Source files/second_pass.c")
