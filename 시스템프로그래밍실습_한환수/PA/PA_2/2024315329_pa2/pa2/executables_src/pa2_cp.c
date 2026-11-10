#include "util.h"

static int cp_one(const char *src, const char *dst) {
    struct stat ss, ds;
    if (stat(src, &ss) < 0) {
        fprintf(stderr, "pa2_cp: cannot stat '%s': %s\n", src, strerror(errno));
        return 1;
    }
    if (stat(dst, &ds) == 0 && ss.st_dev == ds.st_dev && ss.st_ino == ds.st_ino) {
        fprintf(stderr, "pa2_cp: '%s' and '%s' are the same file\n", src, dst);
        return 1;
    }
    int in = open(src, O_RDONLY);
    if (in < 0) {
        fprintf(stderr, "pa2_cp: cannot open '%s' for reading: %s\n", src, strerror(errno));
        return 1;
    }
    // Do not truncate until comparing inode identities above.
    int out = open(dst, O_WRONLY | O_CREAT | O_TRUNC, ss.st_mode & 0777);
    if (out < 0) {
        fprintf(stderr, "pa2_cp: cannot create regular file '%s': %s\n", dst, strerror(errno));
        close(in);
        return 1;
    }
    int err = pa2_copy_fd(in, out);
    if (err < 0) {
        fprintf(stderr, "pa2_cp: %s\n", strerror(errno));
    }
    if (close(in) < 0) err = -1;
    if (close(out) < 0) err = -1;
    return err < 0;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        fprintf(stderr, "pa2_cp: missing file operand\n"); return 1;
    }
    if (argc == 2) {
        fprintf(stderr, "pa2_cp: missing destination file operand after '%s'\n", argv[1]); return 1;
    }
    const char *target = argv[argc - 1];
    struct stat st;
    int exists = stat(target, &st) == 0;
    int isdir = exists && S_ISDIR(st.st_mode);
    if (!isdir && argc > 3) {
        fprintf(stderr, "pa2_cp: target '%s' is not a directory\n", target);
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
        fprintf(stderr, "pa2_cp: cannot %s '%s': Not a directory\n",
                normal_file ? "stat" : "create regular file", target);
        return 1;
    }
    int failed = 0;
    for (int i = 1; i < argc - 1; i++) {
        char *allocated = isdir ? pa2_dest_path(target, argv[i]) : NULL;
        if (isdir && !allocated) {
            fprintf(stderr, "pa2_cp: %s\n", strerror(errno)); failed = 1; continue;
        }
        const char *dest = isdir ? allocated : target;
        failed |= cp_one(argv[i], dest);
        free(allocated);
    }
    return failed;
}
