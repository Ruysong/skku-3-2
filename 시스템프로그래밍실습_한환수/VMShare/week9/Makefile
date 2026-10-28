CC = gcc
CFLAGS = -D_POSIX_C_SOURCE=200809L -std=c11 -Wall -Wextra -O2

.PHONY: all clean

all: w9

w9: w9.c
	$(CC) $(CFLAGS) -o w9 w9.c

clean:
	rm -f w9
