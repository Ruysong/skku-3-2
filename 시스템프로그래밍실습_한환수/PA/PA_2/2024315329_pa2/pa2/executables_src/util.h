#ifndef PA2_UTIL_H
#define PA2_UTIL_H
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static inline int pa2_write_all(int fd, const void *data, size_t size) {
    const unsigned char *p = data;
    while (size) {
        ssize_t n = write(fd, p, size);
        if (n < 0 && errno == EINTR) continue;
        if (n <= 0) return -1;
        p += n;
        size -= (size_t)n;
    }
    return 0;
}
static inline const char *pa2_base(const char *p) {
    const char *slash = strrchr(p, '/');
    return slash ? slash + 1 : p;
}
static inline char *pa2_dest_path(const char *dir, const char *src) {
    const char *base = pa2_base(src);
    size_t a = strlen(dir), b = strlen(base);
    if (a > SIZE_MAX - b - 2) { errno = ENAMETOOLONG; return NULL; }
    char *path = malloc(a + b + 2);
    if (!path) return NULL;
    memcpy(path, dir, a);
    if (a && dir[a - 1] != '/') path[a++] = '/';
    memcpy(path + a, base, b + 1);
    return path;
}
static inline int pa2_ends_slash(const char *p) {
    size_t n = strlen(p);
    return n && p[n - 1] == '/';
}
static inline int pa2_copy_fd(int src, int dst) {
    char buf[32768];
    while (1) {
        ssize_t n = read(src, buf, sizeof buf);
        if (n < 0 && errno == EINTR) continue;
        if (n < 0) return -1;
        if (n == 0) return 0;
        if (pa2_write_all(dst, buf, (size_t)n) == -1) return -1;
    }
}
#endif
