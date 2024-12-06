# Compiler settings
CC := gcc
LINKER := $(CC)
CFLAGS := -O3 -Wall -m64 -msse3
DEBUG_FLAGS := -g -O0

# Directory paths
SRC_DIR := $(ROOT_DIR)/src
BIN_DIR := $(ROOT_DIR)/bin
LIB_DIR := $(ROOT_DIR)/lib
RAYLIB_SRC := $(ROOT_DIR)/raylib/src

# Shared library paths
BLAS_LIB := $(HOME)/blis/lib/libblis.a
FLAME_LIB := $(HOME)/libflame/lib/libflame.a
FLAME_INC := $(HOME)/libflame/include

# Linker flags
LFLAGS := -L$(LIB_DIR)/ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
LFLAGS += -m64 -lm -fopenmp

# Include flags
IFLAGS := -I$(ROOT_DIR)/include/
IFLAGS += -I$(FLAME_INC) -I$(CBLAS_INC)
IFLAGS += $(FLAME_LIB) $(BLAS_LIB)

# Main library name
TARGET = fqam.o