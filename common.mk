# --- Directory Paths ----
# Root Directories
TEST_DIR   := $(ROOT_DIR)/tests
SRC_DIR    :=	$(ROOT_DIR)/src
BIN_DIR    := $(ROOT_DIR)/bin
LIB_DIR    := $(ROOT_DIR)/lib
RAYLIB_SRC := $(ROOT_DIR)/raylib/src

# Main library paths
TARGET   := fqam.o
FQAM_LIB := $(BIN_DIR)/$(TARGET)
FQAM_INC := $(ROOT_DIR)/include/

# Shared library paths
BLAS_LIB  := $(HOME)/blis/lib/libblis.a
FLAME_LIB := $(HOME)/libflame/lib/libflame.a
FLAME_INC := $(HOME)/libflame/include/

# ---- Compiler settings -----
CC          := gcc
LINKER      := $(CC)
CFLAGS      := -O3 -Wall -m64 -msse3
DEBUG_FLAGS := -g -O0

# Include flags
IFLAGS := -I$(FQAM_INC)
IFLAGS += -I$(FLAME_INC) 