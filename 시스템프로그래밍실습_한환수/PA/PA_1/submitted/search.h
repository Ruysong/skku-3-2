#ifndef SEARCH_H
#define SEARCH_H

#include <stdlib.h>

int search_one_word(int fd, const char *query, size_t query_len);

int search_multi_words(int fd,
                       const char *query,
                       size_t query_len);



int search_phrase(int fd,
                  const char *phrase,
                  size_t phrase_len);

int search_pattern(int fd,
                   const char *pattern,
                   size_t pattern_len);

#endif


