# Compiler and flags
CC = gcc
CFLAGS = -Wall -ansi -pedantic

# Paths
ROOT_DIR = .
SRC_DIR = $(ROOT_DIR)/Source_files
INC_DIR = $(ROOT_DIR)/Header_files

# Include path
INCLUDES = -I$(INC_DIR)

# Source files
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

# Object files
OBJ = $(SRC:.c=.o)

# Target executable
TARGET = final_project

# Default rule
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

# Compile .c to .o
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(wildcard $(INC_DIR)/*.h)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Run
run: $(TARGET)
	./$(TARGET)

# Clean
clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)

.PHONY: all run clean