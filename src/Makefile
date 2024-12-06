include ../common.mk

# Get target files for compilation
SRC_BIN_DIR := bin
SRCS := $(shell find . -name '*.c')
OBJS := $(SRCS:./%.c=$(SRC_BIN_DIR)/%.o)

# Create output directories
$(shell mkdir -p $(sort $(dir $(OBJS))))

.PHONY: build debug build_raylib clean_raylib clean

# Build targets
build: $(BIN_DIR)/$(TARGET)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(BIN_DIR)/debug_$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJS)
	$(CC) -r $(OBJS) -o $@

$(SRC_BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(IFLAGS) $(CFLAGS) $< -o $@

clean:
	rm -rf ./$(SRC_BIN_DIR)