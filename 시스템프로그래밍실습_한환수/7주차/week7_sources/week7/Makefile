CC = gcc
CFLAGS = -std=gnu11 -Wall -Wextra -O2
LDLIBS = -ledit

w7: week7.c helper.c helper.h
	$(CC) $(CFLAGS) -o $@ week7.c helper.c $(LDLIBS)

clean:
	rm -f w7

.PHONY: clean
