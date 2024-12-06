include ../common.mk

# Directories
TEST_BIN_DIR := $(BIN_DIR)

# Test source and binary files
TESTS := $(wildcard $(TEST_DIR)/*.c)
TEST_BINS := $(patsubst $(TEST_DIR)/%.c, $(TEST_BIN_DIR)/%.x, $(TESTS))

# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -O2
LFLAGS := -L$(LIB_DIR) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -m64 -fopenmp

# Build target
build: $(TEST_BINS)

# Pattern rule for test binaries
$(TEST_BIN_DIR)/test_%.x: $(TEST_DIR)/test_%.o
	$(LINKER) $(FQAM_LIB) $< $(IFLAGS) $(CFLAGS) $(FLAME_LIB) $(BLAS_LIB) $(LFLAGS) -o $@

# Pattern rule for test object files
$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) $(IFLAGS) -D ASSERTF_DEF_ONCE -c $< -o $@


