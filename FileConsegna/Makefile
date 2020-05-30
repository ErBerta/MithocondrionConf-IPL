CC=gcc
LIB_P=lib/
CFLAGS= -Ilib -Wall -g --ansi --pedantic -g3 -O3 -fsanitize=address -fsanitize=undefined -std=gnu89 -Wextra
LD_FLAGS=-lm -lz

MAIN_EXEC=main_iplib.c
OUT=main
TESTER_EXEC=main_tester.sh
IMG_DIR=images/
#%.o: %.c $(DEPS)
#	$(CC) -c -o $@ $< $(CFLAGS)

all: libs link comp
	@echo "Compilation completed"

libs:
	mkdir -p $(LIB_P)
	$(CC) -Wall -c $(LIB_P)bmp.c -o $(LIB_P)bmp.o $(LD_FLAGS)
	$(CC) -Wall -c $(LIB_P)ip_lib.c -o $(LIB_P)ip_lib.o $(LD_FLAGS)

comp:
	$(CC) $(MAIN_EXEC) -o $(OUT) $(LIB_P)bmp.o $(LIB_P)ip_lib.o -Ilib $(LD_FLAGS) $(CFLAGS)

link:
	ar rcs $(LIB_P)/libbmp.a $(LIB_P)/bmp.o

test:
	./main_tester.sh

cleanup:
	./cleanup.sh
