#include "util.h"

static int head_stream(int fd, const char *name, unsigned long long lines) {
    char buf[32768];
    if (lines == 0) return 0;
    while (lines) {
        ssize_t n = read(fd, buf, sizeof buf);
        if (n < 0 && errno == EINTR) continue;
        if (n < 0) {
            fprintf(stderr, "pa2_head: error reading '%s': %s\n", name, strerror(errno));
            return 1;
        }
        if (n == 0) return 0;
        size_t want = (size_t)n;
        for (size_t i = 0; i < (size_t)n; i++) {
            if (buf[i] == '\n' && --lines == 0) { want = i + 1; break; }
        }
        if (pa2_write_all(STDOUT_FILENO, buf, want) < 0) {
            fprintf(stderr, "pa2_head: %s\n", strerror(errno));
            return 1;
        }
    }
    return 0;
}
int main(int argc, char **argv) {
    unsigned long long lines = 10;
    int opt;
    while ((opt = getopt(argc, argv, "n:")) != -1) {
        if (opt != 'n') return 1;
        if (!optarg[0] || optarg[0] == '-') {
            fprintf(stderr, "pa2_head: invalid number of lines: '%s'\n", optarg);
            return 1;
        }
        char *end;
        errno = 0;
        unsigned long long v = strtoull(optarg, &end, 10);
        if (errno || end == optarg || *end) {
            fprintf(stderr, "pa2_head: invalid number of lines: '%s'\n", optarg);
            return 1;
        }
        lines = v;
    }
    const char *name = (optind < argc) ? argv[optind] : "-";
    if (!strcmp(name, "-")) return head_stream(STDIN_FILENO, "standard input", lines);
    int fd = open(name, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "pa2_head: cannot open '%s' for reading: %s\n", name, strerror(errno));
        return 1;
    }
    int result = head_stream(fd, name, lines);
    close(fd);
    return result;
}
