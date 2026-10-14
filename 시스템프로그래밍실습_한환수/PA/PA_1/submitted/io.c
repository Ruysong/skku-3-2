#include "io.h"
#include <unistd.h>
#include <errno.h>

int line_init(LineBuffer *line){
	line->capacity =128;
	line->length =0;
	line->data = malloc(line->capacity);

	if (line->data ==NULL) return -1;

	line->data[0] = '\0';
	

	return 0;
}

void line_clear(LineBuffer *line)
{
    line->length = 0;
    line->data[0] = '\0';
}

void line_free(LineBuffer *line)
{
    free(line->data);
    line->data = NULL;
    line->length = 0;
    line->capacity = 0;
}
int line_append(LineBuffer *line, char ch)
{
    if (line->length + 1 >= line->capacity) {
        size_t new_capacity = line->capacity * 2;
        char *temp = realloc(line->data, new_capacity);

        if (temp == NULL)
            return -1;

        line->data = temp;
        line->capacity = new_capacity;
    }

    line->data[line->length] = ch;
    line->length++;
    line->data[line->length] = '\0';

    return 0;
}

int read_query(char *query, size_t *length)
{
    static char input_buf[8192];
    static size_t pos = 0;
    static size_t end = 0;

    *length = 0;

    while (1) {
        if (pos == end) {
            ssize_t n = read(0, input_buf, sizeof(input_buf));

            if (n < 0) {
                if (errno == EINTR)
                    continue;
                return -1;
            }

            if (n == 0) {
                if (*length == 0)
                    return 0;

                break;
            }

            pos = 0;
            end = n;
        }

        char ch = input_buf[pos++];

        if (ch == '\n')
            break;

        if (*length >= 4096)
            return -1;

        query[*length] = ch;
        (*length)++;
    }

    /* Windows CRLF query 입력도 정상 처리한다. */
    if (*length > 0 && query[*length - 1] == '\r')
        (*length)--;

    query[*length] = '\0';
    return 1;
}
