#include "util.h"
#include <stdint.h>

static int tail_stream(int fd, const char *name, unsigned long long lines) {
    size_t size = 0, cap = 0;
    unsigned char *data = NULL;
    char buf[32768];
    int err = 0;
    for (;;) {
        ssize_t n = read(fd, buf, sizeof buf);
        if (n < 0 && errno == EINTR) continue;
        if (n < 0) {
            fprintf(stderr, "pa2_tail: error reading '%s': %s\n", name, strerror(errno));
            err = 1; break;
        }
        if (!n) break;
        if ((size_t)n > SIZE_MAX - size) { errno = ENOMEM; err = 1; break; }
        size_t needed = size + (size_t)n;
        if (needed > cap) {
            size_t newcap = cap ? cap : 32768;
            while (newcap < needed) {
                if (newcap > SIZE_MAX / 2) { newcap = needed; break; }
                newcap *= 2;
            }
            unsigned char *tmp = realloc(data, newcap);
            if (!tmp) { err = 1; break; }
            data = tmp; cap = newcap;
        }
        memcpy(data + size, buf, (size_t)n);
        size = needed;
    }
    if (!err && lines && size) {
        size_t pos = size;
        if (data[size - 1] == '\n') pos--;
        unsigned long long found = 0;
        while (pos > 0) {
            pos--;
            if (data[pos] == '\n') {
                if (++found == lines) { pos++; break; }
            }
        }
        if (pa2_write_all(STDOUT_FILENO, data + pos, size - pos) == -1) err = 1;
    }
    if (err && errno == ENOMEM) fprintf(stderr, "pa2_tail: %s\n", strerror(errno));
    free(data);
    return err;
}
int main(int argc, char **argv) {
    unsigned long long lines = 10;
    int opt;
    while ((opt = getopt(argc, argv, "n:")) != -1) {
        if (opt != 'n') return 1;
        if (!optarg[0] || optarg[0] == '-') {
            fprintf(stderr, "pa2_tail: invalid number of lines: '%s'\n", optarg); return 1;
        }
        char *end;
        errno = 0;
        unsigned long long v = strtoull(optarg, &end, 10);
        if (errno || end == optarg || *end) {
            fprintf(stderr, "pa2_tail: invalid number of lines: '%s'\n", optarg); return 1;
        }
        lines = v;
    }
    const char *name = optind < argc ? argv[optind] : "-";
    if (!strcmp(name, "-")) return tail_stream(STDIN_FILENO, "standard input", lines);
    int fd = open(name, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "pa2_tail: cannot open '%s' for reading: %s\n", name, strerror(errno));
        return 1;
    }
    int result = tail_stream(fd, name, lines);
    close(fd);
    return result;
}
