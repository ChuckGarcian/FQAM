SRC_PATH := ./src
BIN_PATH := ./bin

BLAS_LIB   := $(HOME)/blis/lib/libblis.a
FLAME_LIB  := $(HOME)/libflame/lib/libflame.a
FLAME_INC  := $(HOME)/libflame/include

# indicate where the object files are to be created
CC         := gcc
LINKER     := $(CC)
CFLAGS     := -O3 -Wall -I$(FLAME_INC) -I$(CBLAS_INC) -m64 -msse3 
FFLAGS     := $(CFLAGS) 
LDFLAGS    := -m64 -lm -fopenmp

# add files in which your implementations are coded
TEST_OBJS    := driver.o \

TEST_OBJS  := $(addprefix $(SRC_PATH)/, $(TEST_OBJS))

build: $(TEST_OBJS)
	$(LINKER) $(TEST_OBJS) $(FLAME_LIB) $(LAPACK_LIB) $(BLAS_LIB) $(LDFLAGS) -o ./$(BIN_PATH)/driver.x

run: 
	./$(BIN_PATH)/driver.x

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.f
	$(FC) $(FFLAGS) -c $< -o $@

clean:
	rm -f *.o $(OBJ_PATH)/*.o *~ core *.x