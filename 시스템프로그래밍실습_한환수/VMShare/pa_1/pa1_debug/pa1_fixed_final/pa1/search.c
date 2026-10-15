#include "search.h"
#include "io.h"
#include <unistd.h>
#include <errno.h>

/* ASCII 대문자를 소문자로 변환 */
static char lower_ascii(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c + ('a' - 'A');

    return c;
}

/* 길이와 각 문자가 모두 같으면 1 */
static int word_equal(const char *a, size_t a_len,
                      const char *b, size_t b_len)
{
    if (a_len != b_len)
        return 0;

    for (size_t i = 0; i < a_len; i++) {
        if (lower_ascii(a[i]) != lower_ascii(b[i]))
            return 0;
    }

    return 1;
}
/* 현재 줄에 특정 단어가 있는지 확인 */
static int contains_word(const LineBuffer *line,
                         const char *word, size_t word_len)
{
    size_t i = 0;

    while (i < line->length) {

        while (i < line->length && line->data[i] == ' ')
            i++;

        if (i == line->length)
            break;

        size_t start = i;

        while (i < line->length && line->data[i] != ' ')
            i++;

        size_t len = i - start;

        if (word_equal(line->data + start, len,
                       word, word_len))
            return 1;
    }

    return 0;
}

/* write()가 일부만 출력해도 끝까지 출력 */
static int write_all(const char *data, size_t len)
{
    size_t written = 0;

    while (written < len) {
        ssize_t n = write(1, data + written, len - written);

        if (n < 0 && errno == EINTR)
            continue;
        if (n <= 0)
            return -1;

        written += n;
    }

    return 0;
}

/* 검색 결과를 모아 출력해 큰 파일에서도 write() 호출을 줄인다. */
static char output_buf[65536];
static size_t output_len = 0;

static int output_flush(void)
{
    if (write_all(output_buf, output_len) < 0)
        return -1;
    output_len = 0;
    return 0;
}

static int output_write(const char *data, size_t len)
{
    size_t pos = 0;
    while (pos < len) {
        if (output_len == sizeof(output_buf) && output_flush() < 0)
            return -1;

        size_t avail = sizeof(output_buf) - output_len;
        size_t take = (len - pos < avail) ? len - pos : avail;
        for (size_t i = 0; i < take; i++)
            output_buf[output_len + i] = data[pos + i];
        output_len += take;
        pos += take;
    }
    return 0;
}

/* 숫자를 출력 버퍼에 문자로 추가 */
static size_t append_number(char *out, size_t pos, size_t value)
{
    char digits[32];
    size_t count = 0;

    do {
        digits[count++] = '0' + value % 10;
        value /= 10;
    } while (value > 0);

    while (count > 0)
        out[pos++] = digits[--count];

    return pos;
}

/* "줄번호:시작인덱스 " 형식으로 출력 */
static int emit_match(size_t line_number, size_t start)
{
    char out[64];
    size_t pos = 0;

    pos = append_number(out, pos, line_number);
    out[pos++] = ':';
    pos = append_number(out, pos, start);
    out[pos++] = ' ';

    return output_write(out, pos);
}

/* Windows CRLF 줄바꿈의 CR을 검색 대상에서 제외한다. */
static void trim_line_cr(LineBuffer *line)
{
    if (line->length > 0 && line->data[line->length - 1] == '\r') {
        line->length--;
        line->data[line->length] = '\0';
    }
}

/* 완성된 줄 하나에서 일치하는 모든 단어 찾기 */
static int search_line(const LineBuffer *line,
                       size_t line_number,
                       const char *query, size_t query_len)
{
    size_t i = 0;

    while (i < line->length) {

        /* 공백 건너뛰기 */
        while (i < line->length && line->data[i] == ' ')
            i++;

        if (i == line->length)
            break;

        size_t start = i;

        /* 현재 단어의 끝 찾기 */
        while (i < line->length && line->data[i] != ' ')
            i++;

        size_t word_len = i - start;

        if (word_equal(line->data + start, word_len,
                       query, query_len)) {

            if (emit_match(line_number, start) < 0)
                return -1;
        }
    }

    return 0;
}
static int search_multi_line(const LineBuffer *line,
                             size_t line_number,
                             const char *query,
                             size_t query_len)
{
    size_t i = 0;

    while (i < query_len) {

        size_t start = i;

        while (i < query_len && query[i] != ' ')
            i++;

        size_t word_len = i - start;

        if (!contains_word(line, query + start, word_len))
            return 0;

        if (i < query_len)
            i++;
    }

    /* 모든 검색어가 현재 줄에 존재함 */
    char out[32];
    size_t pos = 0;

    pos = append_number(out, pos, line_number);
    out[pos++] = ' ';

    return output_write(out, pos);
}

/* Search 3: 완성된 줄 하나에서 구절의 모든 위치 찾기 */
static int search_phrase_line(const LineBuffer *line,
                              size_t line_number,
                              const char *phrase,
                              size_t phrase_len)
{
    if (phrase_len == 0 || phrase_len > line->length)
        return 0;

    for (size_t start = 0;
         start <= line->length - phrase_len;
         start++) {

        size_t end = start + phrase_len;

        /* 단어 중간에서 시작하면 제외 */
        if (start > 0 && line->data[start - 1] != ' ')
            continue;

        /* 단어 중간에서 끝나면 제외 */
        if (end < line->length && line->data[end] != ' ')
            continue;

        /* 구절 전체 비교: 순서와 공백 유지 */
        if (word_equal(line->data + start, phrase_len,
                       phrase, phrase_len)) {

            if (emit_match(line_number, start) < 0)
                return -1;
        }
    }

    return 0;
}
/* Search 4: 현재 줄에서 word1*word2 패턴 검사 */
static int search_pattern_line(const LineBuffer *line,
                               size_t line_number,
                               const char *word1, size_t len1,
                               const char *word2, size_t len2)
{
    size_t i = 0;
    int found_first = 0;

    while (i < line->length) {

        while (i < line->length && line->data[i] == ' ')
            i++;

        if (i == line->length)
            break;

        size_t start = i;

        while (i < line->length && line->data[i] != ' ')
            i++;

        size_t word_len = i - start;

        if (!found_first) {
            if (word_equal(line->data + start, word_len,
                           word1, len1))
                found_first = 1;
        }
        else {
            if (word_equal(line->data + start, word_len,
                           word2, len2)) {

                char out[32];
                size_t pos = 0;

                pos = append_number(out, pos, line_number);
                out[pos++] = ' ';

                return output_write(out, pos);
            }
        }
    }

    return 0;
}

/* 파일 전체에서 단어 검색 */
int search_one_word(int fd, const char *query, size_t query_len)
{
    LineBuffer line;

    if (line_init(&line) < 0)
        return -1;

    char buf[65536];
    size_t line_number = 1;
    int result = -1;

    while (1) {
        ssize_t n = read(fd, buf, sizeof(buf));

        if (n == 0)
            break;

        if (n < 0) {
            if (errno == EINTR)
                continue;
            goto cleanup;
        }

        for (ssize_t i = 0; i < n; i++) {
            char ch = buf[i];

            if (ch == '\n') {
                trim_line_cr(&line);
                if (search_line(&line, line_number,
                                query, query_len) < 0)
                    goto cleanup;

                line_clear(&line);
                line_number++;
            }
            else {
                if (line_append(&line, ch) < 0)
                    goto cleanup;
            }
        }
    }

    /* 마지막 줄에 '\n'이 없는 경우 */
    trim_line_cr(&line);
    if (line.length > 0) {
        if (search_line(&line, line_number,
                        query, query_len) < 0)
            goto cleanup;
    }

    /* query 하나의 검색이 끝났으므로 줄바꿈 */
    if (output_write("\n", 1) < 0 || output_flush() < 0)
        goto cleanup;

    result = 0;

cleanup:
    line_free(&line);
    return result;
}

int search_multi_words(int fd,
                       const char *query,
                       size_t query_len)
{
    LineBuffer line;

    if (line_init(&line) < 0)
        return -1;

    char buf[65536];
    size_t line_number = 1;
    int result = -1;

    while (1) {
        ssize_t n = read(fd, buf, sizeof(buf));

        if (n == 0)
            break;

        if (n < 0) {
            if (errno == EINTR)
                continue;
            goto cleanup;
        }

        for (ssize_t i = 0; i < n; i++) {
            char ch = buf[i];

            if (ch == '\n') {
                trim_line_cr(&line);
                if (search_multi_line(&line, line_number,
                                      query, query_len) < 0)
                    goto cleanup;

                line_clear(&line);
                line_number++;
            }
            else {
                if (line_append(&line, ch) < 0)
                    goto cleanup;
            }
        }
    }

    trim_line_cr(&line);
    if (line.length > 0) {
        if (search_multi_line(&line, line_number,
                              query, query_len) < 0)
            goto cleanup;
    }

    if (output_write("\n", 1) < 0 || output_flush() < 0)
        goto cleanup;

    result = 0;

cleanup:
    line_free(&line);
    return result;
}

/* Search 3: 파일 전체에서 구절 검색 */
int search_phrase(int fd,
                  const char *phrase,
                  size_t phrase_len)
{
    LineBuffer line;

    if (line_init(&line) < 0)
        return -1;

    char buf[65536];
    size_t line_number = 1;
    int result = -1;

    while (1) {
        ssize_t n = read(fd, buf, sizeof(buf));

        if (n == 0)
            break;

        if (n < 0) {
            if (errno == EINTR)
                continue;
            goto cleanup;
        }

        for (ssize_t i = 0; i < n; i++) {
            char ch = buf[i];

            if (ch == '\n') {
                trim_line_cr(&line);
                if (search_phrase_line(&line, line_number,
                                       phrase, phrase_len) < 0)
                    goto cleanup;

                line_clear(&line);
                line_number++;
            }
            else {
                if (line_append(&line, ch) < 0)
                    goto cleanup;
            }
        }
    }

    /* 마지막 줄에 '\n'이 없는 경우 */
    trim_line_cr(&line);
    if (line.length > 0) {
        if (search_phrase_line(&line, line_number,
                               phrase, phrase_len) < 0)
            goto cleanup;
    }

    if (output_write("\n", 1) < 0 || output_flush() < 0)
        goto cleanup;

    result = 0;

cleanup:
    line_free(&line);
    return result;


}

/* Search 4: 파일 전체에서 패턴 검색 */
int search_pattern(int fd,
                   const char *pattern,
                   size_t pattern_len)
{
    size_t star = 0;

    while (star < pattern_len && pattern[star] != '*')
        star++;

    if (star == 0 || star >= pattern_len - 1)
        return -1;

    const char *word1 = pattern;
    size_t len1 = star;

    const char *word2 = pattern + star + 1;
    size_t len2 = pattern_len - star - 1;

    LineBuffer line;

    if (line_init(&line) < 0)
        return -1;

    char buf[65536];
    size_t line_number = 1;
    int result = -1;

    while (1) {
        ssize_t n = read(fd, buf, sizeof(buf));

        if (n == 0)
            break;

        if (n < 0) {
            if (errno == EINTR)
                continue;
            goto cleanup;
        }

        for (ssize_t i = 0; i < n; i++) {
            char ch = buf[i];

            if (ch == '\n') {
                trim_line_cr(&line);
                if (search_pattern_line(&line, line_number,
                                        word1, len1,
                                        word2, len2) < 0)
                    goto cleanup;

                line_clear(&line);
                line_number++;
            }
            else {
                if (line_append(&line, ch) < 0)
                    goto cleanup;
            }
        }
    }

    /* 마지막 줄에 '\n'이 없는 경우 */
    trim_line_cr(&line);
    if (line.length > 0) {
        if (search_pattern_line(&line, line_number,
                                word1, len1,
                                word2, len2) < 0)
            goto cleanup;
    }

    if (output_write("\n", 1) < 0 || output_flush() < 0)
        goto cleanup;

    result = 0;

cleanup:
    line_free(&line);
    return result;
}
