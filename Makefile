CC = gcc
CFLAGS = -Wall -Wextra

all: parse funcion interface

parse: hash_table_index.c
	$(CC) $(FLAGS) $^ -o $@
funcion: funcion.c
	$(CC) $(FLAGS) $^ -o $@
interface: interface.c
	$(CC) $(FLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -f parse funcion interface
