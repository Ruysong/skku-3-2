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
