CC=gcc
CFLAGS=-Wall
TARGET=w3
OBJECTS=main.o plus.o minus.o

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TARGET)

