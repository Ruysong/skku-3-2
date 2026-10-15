#include <unistd.h>
#include <fcntl.h>
#include "io.h"
#include "search.h"

static int is_exit_query(const char *query, size_t length)
{
    const char exit_word[] = "PA1EXIT";

    if (length != 7)
        return 0;

    for (size_t i = 0; i < length; i++) {
        if (query[i] != exit_word[i])
            return 0;
    }

    return 1;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;

    int fd = open(argv[1], O_RDONLY);

    if (fd < 0)
        return 1;

    char query[4097];
    size_t query_len;

    while (1) {
        int status = read_query(query, &query_len);

        if (status == 0)
            break;

        if (status < 0) {
            close(fd);
            return 1;
        }

        if (is_exit_query(query, query_len))
            break;

        /* 새 query마다 파일 처음부터 검색 */
        if (lseek(fd, 0, SEEK_SET) < 0) {
            close(fd);
            return 1;
        }

        /*
         * 여기에 나중에 Search 1~4 호출.
         */

	int has_space = 0;

for (size_t i = 0; i < query_len; i++) {
    if (query[i] == ' ') {
        has_space = 1;
        break;
    }
}
int has_star = 0;

for (size_t i = 0; i < query_len; i++) {
    if (query[i] == '*') {
        has_star = 1;
        break;
    }
}

	int search_result;

if (has_star) {
    search_result = search_pattern(fd, query, query_len);
}
else if (query_len >= 2 &&
         query[0] == '"' &&
         query[query_len - 1] == '"') {

    search_result = search_phrase(
        fd, query + 1, query_len - 2
    );
}
else if (has_space) {
    search_result = search_multi_words(fd, query, query_len);
}
else {
    search_result = search_one_word(fd, query, query_len);
}

if (search_result < 0) {
    close(fd);
    return 1;
}


}

    close(fd);
    return 0;
}
