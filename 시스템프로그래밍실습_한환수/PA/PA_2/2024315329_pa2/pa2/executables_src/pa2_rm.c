#include "util.h"
int main(int argc, char **argv) {
    if (argc == 1) {
        fprintf(stderr, "pa2_rm: missing operand\n");
        return 1;
    }
    int failed = 0;
    for (int i = 1; i < argc; i++) {
        if (unlink(argv[i]) < 0) {
            fprintf(stderr, "pa2_rm: cannot remove '%s': %s\n", argv[i], strerror(errno));
            failed = 1;
        }
    }
    return failed;
}
