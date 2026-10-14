#ifndef IO_H
#define IO_H

#include <stdlib.h>

typedef struct {
	char *data;
	size_t length;
	size_t capacity;
} LineBuffer;

int line_init(LineBuffer *line);
int line_append(LineBuffer *line,char ch);
void line_clear(LineBuffer *line);
void line_free(LineBuffer *line);
int read_query(char *query,size_t *length);

#endif
