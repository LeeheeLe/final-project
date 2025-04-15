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
  $(SRC_DIR)/memory_utility.c

# Executable
TARGET = final_project

# Test files (passed as args to main)
TEST_INPUTS = test_1.txt test_2.txt test_3.txt

.PHONY: all run test clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(SRC)

run: $(TARGET)
	./$(TARGET) $(TEST_INPUTS)

test: run

clean:
	rm -f $(TARGET)
