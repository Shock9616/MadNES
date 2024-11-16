# Directories
SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build
BIN_DIR := $(BUILD_DIR)/bin
TESTS_DIR := tests
CUNIT_INCLUDE_DIR := /opt/homebrew/Cellar/cunit/2.1-3/include/
UNIT_TESTS_DIR := $(TESTS_DIR)/unit
TESTS_BUILD_DIR := $(BUILD_DIR)/tests

# Compiler and flags
CC := clang
CFLAGS := -Wall -I $(INCLUDE_DIR) -I $(TESTS_DIR) -I $(CUNIT_INCLUDE_DIR) -g3 -std=c2x
LDFLAGS := -L/opt/homebrew/Cellar/cunit/2.1-3/lib/ -lcunit

# Source files, object files, and executable
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))
OBJ_FILES_TEST_MODE := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.test.o,$(SRC_FILES))
EXECUTABLE := $(BIN_DIR)/nes

# Test source files and object files
TEST_SRC_FILES := $(wildcard $(UNIT_TESTS_DIR)/*.c)
TEST_OBJ_FILES := $(patsubst $(UNIT_TESTS_DIR)/%.c,$(TESTS_BUILD_DIR)/%.o,$(TEST_SRC_FILES))
TEST_EXECUTABLE := $(BIN_DIR)/tests

# Default target
all: $(EXECUTABLE)

# Link object files into the final executable
$(EXECUTABLE): $(OBJ_FILES) | $(BIN_DIR)
	$(CC) $(OBJ_FILES) -o $@

# Compile each .c file into a .o file for the application
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile each .c file into a .test.o file for testing
$(BUILD_DIR)/%.test.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -DTEST -c $< -o $@

# Compile unit tests
$(TESTS_BUILD_DIR)/%.o: $(UNIT_TESTS_DIR)/%.c | $(TESTS_BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create test executable
$(TEST_EXECUTABLE): $(TEST_OBJ_FILES) $(OBJ_FILES_TEST_MODE) | $(BIN_DIR)
	$(CC) $(OBJ_FILES_TEST_MODE) $(TEST_OBJ_FILES) $(LDFLAGS) -o $@

test: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

# Create directories if they do not exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TESTS_BUILD_DIR):
	mkdir -p $(TESTS_BUILD_DIR)

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
