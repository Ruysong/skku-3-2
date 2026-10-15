// Hint for PA2: You can use this as the base code for cat, head, and tail

#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

enum { BATCH_SIZE = 4028 };

char do_not_touch_this_array[21];
char* to_string(uint64_t integer);
size_t w4_strlen(const char* str);
void get_output_filename(const char* input_filename, char* output_filename);

// Add more functions here if needed
ssize_t write_all(int fd, const char* buf, size_t count);

int main(int argc, const char* argv[]) {
    const char* input_filename = argv[1];
    if (input_filename == NULL) {
        const char* err_msg = "Please enter filename!\n";
        write(STDERR_FILENO, err_msg, w4_strlen(err_msg));
        return 1;
    }

    char output_filename[w4_strlen(input_filename) + 4];
    get_output_filename(input_filename, output_filename);

    uint64_t line_number = 1;

    // TODO: write your code here
        int input_fd = open(input_filename, O_RDONLY);
    if (input_fd < 0) {
        const char* err_msg = "Failed to open input file!\n";
        write(STDERR_FILENO, err_msg, w4_strlen(err_msg));
        return 1;
    }

    int output_fd =
        open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (output_fd < 0) {
        const char* err_msg = "Failed to open output file!\n";
        write(STDERR_FILENO, err_msg, w4_strlen(err_msg));
        close(input_fd);
        return 1;
    }

    char buffer[BATCH_SIZE];
    ssize_t nread;
    bool line_start = true;

    while ((nread = read(input_fd, buffer, BATCH_SIZE)) > 0) {
        for (ssize_t i = 0; i < nread; i++) {

            if (line_start) {
                char* number = to_string(line_number);

                if (write_all(output_fd,
                              number,
                              w4_strlen(number)) < 0) {
                    const char* err_msg = "Write error!\n";
                    write(STDERR_FILENO,
                          err_msg,
                          w4_strlen(err_msg));
                    close(input_fd);
                    close(output_fd);
                    return 1;
                }

                if (write_all(output_fd, " | ", 3) < 0) {
                    const char* err_msg = "Write error!\n";
                    write(STDERR_FILENO,
                          err_msg,
                          w4_strlen(err_msg));
                    close(input_fd);
                    close(output_fd);
                    return 1;
                }

                line_start = false;
            }

            if (write_all(output_fd, &buffer[i], 1) < 0) {
                const char* err_msg = "Write error!\n";
                write(STDERR_FILENO,
                      err_msg,
                      w4_strlen(err_msg));
                close(input_fd);
                close(output_fd);
                return 1;
            }

            if (buffer[i] == '\n') {
                line_number++;
                line_start = true;
            }
        }
    }

    if (nread < 0) {
        const char* err_msg = "Read error!\n";
        write(STDERR_FILENO, err_msg, w4_strlen(err_msg));
        close(input_fd);
        close(output_fd);
        return 1;
    }

    if (close(input_fd) < 0) {
        const char* err_msg = "Failed to close input file!\n";
        write(STDERR_FILENO, err_msg, w4_strlen(err_msg));
        close(output_fd);
        return 1;
    }

    if (close(output_fd) < 0) {
        const char* err_msg = "Failed to close output file!\n";
        write(STDERR_FILENO, err_msg, w4_strlen(err_msg));
        return 1;
    }

    return 0;
}

// helper functions
char* to_string(uint64_t integer) {
    do_not_touch_this_array[20] = '\0';
    for (int i = 19; i >= 0; i--) {
        do_not_touch_this_array[i] = (integer % 10) + '0';
        integer /= 10;
        if (integer == 0) return (char*)&do_not_touch_this_array[i];
    }
    return (char*)do_not_touch_this_array;
}

size_t w4_strlen(const char* str) {
    int len;
    for (len = 0; str[len] != '\0'; len++);
    return len;
}

void get_output_filename(const char* input_filename, char* output_filename) {
    int len = w4_strlen(input_filename);
    int i;

    for (i = 0; i < len; i++) {
        output_filename[i] = input_filename[i];
    }

    output_filename[i++] = '.';
    output_filename[i++] = 'n';
    output_filename[i++] = 'o';
    output_filename[i] = '\0';
}

ssize_t write_all(int fd, const char* buf, size_t count) {
    size_t total = 0;

    while (total < count) {
        ssize_t nwritten =
            write(fd, buf + total, count - total);

        if (nwritten <= 0) {
            return -1;
        }

        total += (size_t)nwritten;
    }

    return (ssize_t)total;
}


