/*
 * File: operations.c
 * -------------------
 * This file defines the operation syntax table and provides a function to retrieve
 * operation details based on the operation's name.
 */
#include "parsing.h"
#include "first_pass.h"
#include "const_tables.h"
#include "errors.h"
#include <stdio.h>
#include <string.h>

/* Default structure representing a non-existent operation */
static const operation_syntax no_operation = {NULL, -1, -1, {0, 0, 0, 0}, {0, 0, 0, 0}};

/*
 * Array of valid operations with their corresponding opcodes and addressing modes.
 * Each operation contains:
 *   - Name (string)
 *   - Opcode (integer)
 *   - Function code (integer)
 *   - Allowed source operand addressing modes (array of four values)
 *   - Allowed destination operand addressing modes (array of four values)
 */
static const operation_syntax operations[] = {
    {"mov", 0, 0, {1, 1, 0, 1}, {0, 1, 0, 1}},
    {"cmp", 1, 0, {1, 1, 0, 1}, {1, 1, 0, 1}},
    {"add", 2, 1, {1, 1, 0, 1}, {0, 1, 0, 1}},
    {"sub", 2, 2, {1, 1, 0, 1}, {0, 1, 0, 1}},
    {"lea", 4, 0, {0, 1, 0, 0}, {0, 1, 0, 1}},
    {"clr", 5, 1, {0, 0, 0, 0}, {0, 1, 0, 1}},
    {"not", 5, 2, {0, 0, 0, 0}, {0, 1, 0, 1}},
    {"inc", 5, 3, {0, 0, 0, 0}, {0, 1, 0, 1}},
    {"dec", 5, 4, {0, 0, 0, 0}, {0, 1, 0, 1}},
    {"jmp", 9, 1, {0, 0, 0, 0}, {0, 1, 1, 0}},
    {"bne", 9, 2, {0, 0, 0, 0}, {0, 1, 1, 0}},
    {"jsr", 9, 3, {0, 0, 0, 0}, {0, 1, 1, 0}},
    {"red", 12, 0, {0, 0, 0, 0}, {0, 1, 0, 1}},
    {"prn", 13, 0, {0, 0, 0, 0}, {1, 1, 0, 1}},
    {"rts", 14, 0, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {"stop", 15, 0, {0, 0, 0, 0}, {0, 0, 0, 0}}
};

/* Number of defined operations */
const int num_of_operations = sizeof(operations) / sizeof(operation_syntax);

/*
 * Function: find_operation
 * -------------------------
 * Searches for an operation by its name and returns its corresponding structure.
 *
 * Parameters:
 *   - name: The name of the operation to search for.
 *
 * Returns:
 *   - The corresponding operation_syntax structure if found.
 *   - A default "no_operation" structure if not found, triggering an error.
 */
operation_syntax find_operation(char *name) {
    int i;
    for (i = 0; i < num_of_operations; i++) {
        if (strcmp(operations[i].name, name) == 0) {
            return operations[i];
        }
    }
    NON_EXISTANT_NAME(name);  /* Trigger an error for a non-existent operation */
    return no_operation;       /* Return the default no-operation structure */
}