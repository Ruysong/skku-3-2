CC = gcc
CFLAGS = -Wall -Wextra -O2

pa1: main.o io.o search.o
	$(CC) $(CFLAGS) main.o io.o search.o -o pa1

main.o: main.c io.h search.h
	$(CC) $(CFLAGS) -c main.c

io.o: io.c io.h
	$(CC) $(CFLAGS) -c io.c

search.o: search.c search.h io.h
	$(CC) $(CFLAGS) -c search.c

clean:
	rm -f pa1 main.o io.o search.o

.PHONY: clean
