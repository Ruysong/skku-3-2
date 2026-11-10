#include "util.h"

static int cat_stream(int fd, const char *name) {
    char buf[32768];
    for (;;) {
        ssize_t n = read(fd, buf, sizeof buf);
        if (n < 0 && errno == EINTR) continue;
        if (n < 0) {
            fprintf(stderr, "pa2_cat: %s: %s\n", name, strerror(errno));
            return 1;
        }
        if (n == 0) return 0;
        if (pa2_write_all(STDOUT_FILENO, buf, (size_t)n) < 0) {
            fprintf(stderr, "pa2_cat: %s\n", strerror(errno));
            return 1;
        }
    }
}
int main(int argc, char **argv) {
    int failed = 0;
    if (argc == 1) return cat_stream(STDIN_FILENO, "standard input");
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-")) {
            failed |= cat_stream(STDIN_FILENO, "standard input");
            continue;
        }
        int fd = open(argv[i], O_RDONLY);
        if (fd < 0) {
            fprintf(stderr, "pa2_cat: %s: %s\n", argv[i], strerror(errno));
            failed = 1;
            continue;
        }
        failed |= cat_stream(fd, argv[i]);
        if (close(fd) < 0) failed = 1;
    }
    return failed;
}
