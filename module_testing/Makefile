CC=gcc
CFLAGS= -ILib/ -Wall -g --ansi --pedantic -g3 -O3 -fsanitize=address -fsanitize=undefined -std=gnu89 -Wextra
LIB_P=lib/
LD_FLAGS=-lm -lz

MAIN_EXEC=module_tester.c
OUT=exec.out

#%.o: %.c $(DEPS)
#	$(CC) -c -o $@ $< $(CFLAGS)

all: libs link comp
	@echo "Compilation completed"

libs:
	mkdir -p lib
	$(CC) -Wall -c bitmap/bmp.c -o lib/bmp.o $(LD_FLAGS)
	$(CC) -Wall -c bitmap/ip_lib.c -o lib/ip_lib.o $(LD_FLAGS)

comp:
#	$(CC) $(MAIN_EXEC) -o $(OUT) -Llib -lbmp $(LD_FLAGS)
	$(CC) $(MAIN_EXEC) -o $(OUT) lib/bmp.o lib/ip_lib.o -Ibitmap $(LD_FLAGS) $(CFLAGS)

link:
	ar rcs lib/libbmp.a lib/bmp.o

memtest:
	valgrind -v --leak-check=full ./$(OUT)

exe:
	./$(OUT)
