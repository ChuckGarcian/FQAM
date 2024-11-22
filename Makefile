SRC_PATH := src
BIN_PATH := bin
BLAS_LIB   := $(HOME)/blis/lib/libblis.a
FLAME_LIB  := $(HOME)/libflame/lib/libflame.a
FLAME_INC  := $(HOME)/libflame/include

CC         := gcc
LINKER     := $(CC)
CFLAGS     := -O3 -Wall -I$(FLAME_INC) -I$(CBLAS_INC) -m64 -msse3 
FFLAGS     := $(CFLAGS) 
LDFLAGS    := -m64 -lm -fopenmp

SRC_OBJS    := driver.o
SRC_OBJS    := $(addprefix $(BIN_PATH)/, $(SRC_OBJS))

# UTIL_OBJS    := list.o
# UTIL_OBJS    := $(addprefix $(BIN_PATH)/, $(UTIL_OBJS))

build: $(SRC_OBJS) 
	$(LINKER) $(SRC_OBJS) $(FLAME_LIB) $(BLAS_LIB) $(LDFLAGS) -o ./$(BIN_PATH)/driver.x

run: 
	./$(BIN_PATH)/driver.x

$(BIN_PATH)/%.o: $(SRC_PATH)/%.c 
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $(BIN_PATH)/$@

clean:
	rm -f $(BIN_PATH)/*.o $(BIN_PATH)/*.x