CC=gcc
LIB_P=lib/
CFLAGS= -I$(LIB_P) -Wall -g --ansi --pedantic -g3 -O3 -fsanitize=address -fsanitize=undefined -std=gnu89 -Wextra
LD_FLAGS=-lm -lz

MAIN_EXEC=module_tester.c
OUT=exec.out

#%.o: %.c $(DEPS)
#	$(CC) -c -o $@ $< $(CFLAGS)

all: libs link comp
	@echo "Compilation completed"

libs:
	mkdir -p lib
	$(CC) -Wall -c $(LIB_P)bmp.c -o $(LIB_P)bmp.o $(LD_FLAGS)
	$(CC) -Wall -c $(LIB_P)ip_lib.c -o $(LIB_P)ip_lib.o $(LD_FLAGS)

comp:
#	$(CC) $(MAIN_EXEC) -o $(OUT) -Llib -lbmp $(LD_FLAGS)
	$(CC) $(MAIN_EXEC) -o $(OUT) $(LIB_P)bmp.o $(LIB_P)ip_lib.o -I$(LIB_P) $(LD_FLAGS) $(CFLAGS)

link:
	ar rcs $(LIB_P)libbmp.a $(LIB_P)bmp.o

memtest:
	valgrind -v --leak-check=full ./$(OUT)

exe:
	./$(OUT)
