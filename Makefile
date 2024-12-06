# Directories
SRC_DIR := src
BIN_DIR := bin
LIB_DIR := lib
RAYLIB_SRC := raylib/src

# External libraries
BLAS_LIB := $(HOME)/blis/lib/libblis.a
FLAME_LIB := $(HOME)/libflame/lib/libflame.a
FLAME_INC := $(HOME)/libflame/include

# Compiler settings
CC := gcc
LINKER := $(CC)
CFLAGS := -O3 -Wall -I$(FLAME_INC) -I$(CBLAS_INC) -m64 -msse3 
FFLAGS := $(CFLAGS)
DEBUG_FLAGS := -g -O0

# Source and object files
SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)

# Create output directories
$(shell mkdir -p $(sort $(dir $(OBJS))))

# Linker and include flags
LFLAGS := -L./$(LIB_DIR)/ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
LFLAGS += -m64 -lm -fopenmp
IFLAGS := -I./include/

# Default target
default: run

# Build targets
build: $(BIN_DIR)/app.x

debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(BIN_DIR)/app.x

$(BIN_DIR)/app.x: $(OBJS)
	mkdir -p $(BIN_DIR)
	$(CC) $^ $(IFLAGS) $(CFLAGS) $(FLAME_LIB) $(BLAS_LIB) $(LFLAGS) -o $@

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(IFLAGS) $(CFLAGS) $< -o $@

# Raylib targets
build_raylib:
	make -C $(RAYLIB_SRC) PLATFORM=PLATFORM_DESKTOP && \
	cp $(RAYLIB_SRC)/libraylib.a $(LIB_DIR)

# Cleaning targets
clean_raylib:
	make -C $(RAYLIB_SRC) clean
	$(RM) $(LIB_DIR)/libraylib.a

clean:
	$(RM) -rf $(BIN_DIR) $(OBJS)

# Run target
run: build
	$(BIN_DIR)/app.x

.PHONY: all build_raylib clean_raylib clean default run