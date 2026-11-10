
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int status = 0;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s FILE...\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {

        if (unlink(argv[i]) == -1) {
            perror(argv[i]);
            status = 1;
            continue;
        }
    }

    return status;
}
