# Compiler and flags
CC = gcc
CFLAGS = -Wall -ansi -pedantic -g -std=c90
INCLUDES = -IHeader\ files

# Source directory (escaped space for Make compatibility)
SRC_DIR = Source\ files

# All source files
SRC = \
  $(SRC_DIR)/entry_table.c \
  $(SRC_DIR)/const_tables.c \
  $(SRC_DIR)/first_pass.c \
  $(SRC_DIR)/input.c \
  $(SRC_DIR)/intern_table.c \
  $(SRC_DIR)/label_table.c \
  $(SRC_DIR)/main.c \
  $(SRC_DIR)/parsing.c \
  $(SRC_DIR)/preprocessor.c \
  $(SRC_DIR)/second_pass.c \
  $(SRC_DIR)/memory_utility.c \
  $(SRC_DIR)/utility.c \
  $(SRC_DIR)/handle_text.c

# Executable
TARGET = final_project

# Test files (passed as args to main)
VALID_TEST_INPUTS = example1 example2 example3 example4 example5 ps test
INVALID_TEST_INPUTS = invalid_example1 invalid_example2 invalid_example3 invalid_example4 invalid_example5
.PHONY: all run test clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(SRC)

run: $(TARGET)
	@echo -----------------------
	@echo testing valid inputs:
	@echo -----------------------
	@cd "Valid input output ex" && ../$(TARGET) $(VALID_TEST_INPUTS)
	@echo -----------------------
	@echo testing invalid inputs
	@echo -----------------------
	@cd "Invalid input output ex" && ../$(TARGET) $(INVALID_TEST_INPUTS)

test: run

clean:
	rm -f $(TARGET)
