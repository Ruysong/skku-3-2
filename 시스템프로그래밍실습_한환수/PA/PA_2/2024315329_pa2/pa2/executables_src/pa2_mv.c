#include "util.h"

// NAME is displayed in errors instead of the full source pathname.
static int mv_one(const char *src, const char *dst) {
    struct stat ss, ds;
    const char *name = pa2_base(src);
    if (stat(src, &ss) == -1) {
        fprintf(stderr, "pa2_mv: cannot stat '%s': %s\n", name, strerror(errno));
        return 1;
    }
    if (stat(dst, &ds) == 0 && ss.st_dev == ds.st_dev && ss.st_ino == ds.st_ino) {
        fprintf(stderr, "pa2_mv: '%s' and '%s' are the same file\n", name, pa2_base(dst));
        return 1;
    }
    if (rename(src, dst) == 0) return 0;
    int error = errno;
    if (error == EXDEV && S_ISREG(ss.st_mode)) {
        // Cross-filesystem fallback for regular files.
        int in = open(src, O_RDONLY);
        if (in < 0) error = errno;
        else {
            int out = open(dst, O_WRONLY | O_CREAT | O_TRUNC, ss.st_mode & 0777);
            if (out < 0) error = errno;
            else {
                int result = pa2_copy_fd(in, out);
                if (result < 0) error = errno;
                else error = 0;
                if (close(out) < 0 && error == 0) error = errno;
                if (error == 0 && unlink(src) < 0) error = errno;
            }
            close(in);
            if (error == 0) return 0;
        }
    }
    if (error == EINVAL && S_ISDIR(ss.st_mode)) {
        fprintf(stderr, "pa2_mv: cannot move '%s' to a subdirectory of itself, '%s'\n", name, dst);
    } else {
        fprintf(stderr, "pa2_mv: cannot move '%s' to '%s': %s\n", name, dst, strerror(error));
    }
    return 1;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        fprintf(stderr, "pa2_mv: missing file operand\n"); return 1;
    }
    if (argc == 2) {
        fprintf(stderr, "pa2_mv: missing destination file operand after '%s'\n", pa2_base(argv[1])); return 1;
    }
    const char *target = argv[argc - 1];
    struct stat st;
    int exists = stat(target, &st) == 0;
    int isdir = exists && S_ISDIR(st.st_mode);
    if (!isdir && argc > 3) {
        fprintf(stderr, "pa2_mv: target '%s' is not a directory\n", target);
        return 1;
    }
    if (!isdir && pa2_ends_slash(target)) {
        // Distinguish 'existing regular file/' from 'missing directory/'.
        char *trimmed = strdup(target);
        if (!trimmed) return 1;
        size_t len = strlen(trimmed);
        while (len > 1 && trimmed[len - 1] == '/') trimmed[--len] = '\0';
        struct stat underlying;
        int normal_file = stat(trimmed, &underlying) == 0 && !S_ISDIR(underlying.st_mode);
        free(trimmed);
        if (normal_file) {
            fprintf(stderr, "pa2_mv: cannot stat '%s': Not a directory\n", target);
        } else {
            fprintf(stderr, "pa2_mv: cannot move '%s' to '%s': Not a directory\n",
                    pa2_base(argv[1]), target);
        }
        return 1;
    }
    int failed = 0;
    for (int i = 1; i < argc - 1; i++) {
        char *allocated = isdir ? pa2_dest_path(target, argv[i]) : NULL;
        if (isdir && !allocated) {
            fprintf(stderr, "pa2_mv: %s\n", strerror(errno)); failed = 1; continue;
        }
        failed |= mv_one(argv[i], isdir ? allocated : target);
        free(allocated);
    }
    return failed;
}
