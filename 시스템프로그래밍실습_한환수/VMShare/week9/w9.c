#include <errno.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

volatile sig_atomic_t should_exit = 0;
int error_code = 0;

#define ACK_TYPE 1000000ULL
#define ERASE_CURRENT_LINE "\33[2K\r"

typedef struct {
    long msgtype;                 // Receiver's user ID
    int sender_id;                // Sender's user ID
    char text[512];
} msgbuf;

typedef struct {
    long msgtype;                 // ACK_TYPE + original sender's ID
    char timestamp[512];
} msgbuf_ack;

void get_input(char **buf, size_t *size, char *prompt, ...);
int get_id(char *text);
void sigint_handler(int sig);
void setup_handler(void);

int main(void) {
    setup_handler();

    int user_id = get_id("User ID: ");
    if (should_exit) return error_code;
    int receiver_id = get_id("Receiver ID: ");
    if (should_exit) return error_code;

    // Normal message types must not overlap ACK message types.
    if (user_id <= 0 || receiver_id <= 0 ||
        user_id >= (int)ACK_TYPE || receiver_id >= (int)ACK_TYPE ||
        user_id == receiver_id) {
        fprintf(stderr, "User IDs must be distinct positive numbers below %llu.\n",
                ACK_TYPE);
        return 1;
    }

    // Both users must run this program from the same directory.
    key_t key = ftok(".", 'a');
    if (key == (key_t)-1) {
        perror("ftok");
        return 1;
    }

    int qid = msgget(key, IPC_CREAT | 0666);
    if (qid == -1) {
        perror("msgget");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {               // Child: receiver
        while (!should_exit) {
            msgbuf buf = {0};
            ssize_t n = msgrcv(qid, &buf, sizeof(buf) - sizeof(buf.msgtype),
                                user_id, IPC_NOWAIT);
            if (n >= 0) {
                printf(ERASE_CURRENT_LINE "User %d:\t%s", buf.sender_id, buf.text);
                if (strchr(buf.text, '\n') == NULL) putchar('\n');
                printf("User %d:\t", user_id);
                fflush(stdout);

                // Immediately send a read-time ACK to the original sender.
                msgbuf_ack ack = {0};
                ack.msgtype = (long)(ACK_TYPE + buf.sender_id);
                time_t now = time(NULL);
                struct tm *local = localtime(&now);
                if (local == NULL ||
                    strftime(ack.timestamp, sizeof(ack.timestamp),
                             "%Y-%m-%d %H:%M:%S", local) == 0) {
                    fprintf(stderr, "Could not format timestamp\n");
                    error_code = 1;
                    break;
                }
                if (msgsnd(qid, &ack, sizeof(ack) - sizeof(ack.msgtype),
                           IPC_NOWAIT) == -1) {
                    perror("msgsnd (ACK)");
                    error_code = 1;
                    break;
                }
            } else if (errno == EIDRM || errno == EINVAL) {
                // Another user removed the shared queue: stop this client too.
                kill(getppid(), SIGINT);
                break;
            } else if (errno != ENOMSG && errno != EINTR) {
                if (!should_exit) perror("msgrcv (message)");
                error_code = should_exit ? error_code : 1;
                break;
            }

            msgbuf_ack read_time = {0};
            n = msgrcv(qid, &read_time,
                       sizeof(read_time) - sizeof(read_time.msgtype),
                       (long)(ACK_TYPE + user_id), IPC_NOWAIT);
            if (n >= 0) {
                printf(ERASE_CURRENT_LINE "User %d read message at %s\n",
                       receiver_id, read_time.timestamp);
                printf("User %d:\t", user_id);
                fflush(stdout);
            } else if (errno == EIDRM || errno == EINVAL) {
                kill(getppid(), SIGINT);
                break;
            } else if (errno != ENOMSG && errno != EINTR) {
                if (!should_exit) perror("msgrcv (ACK)");
                error_code = should_exit ? error_code : 1;
                break;
            }

            // Avoid spinning at 100% CPU while the queue is empty.
            if (n == -1 && errno == ENOMSG) {
                struct timespec idle = {0, 10000000L}; // 10 ms
                nanosleep(&idle, NULL);
            }
        }
        _exit(error_code);
    }

    // Parent: sender
    while (!should_exit) {
        msgbuf buf = {0};
        buf.msgtype = receiver_id;
        buf.sender_id = user_id;

        char *line = NULL;
        size_t len = 0;
        get_input(&line, &len, "User %d: ", user_id);
        if (should_exit) {
            free(line);
            break;
        }
        if (strcmp(line, "quit\n") == 0 || strcmp(line, "quit") == 0) {
            free(line);
            break;
        }
        if (strlen(line) >= sizeof(buf.text)) {
            fprintf(stderr, "Message too long (maximum %zu bytes).\n",
                    sizeof(buf.text) - 1);
            free(line);
            continue;
        }

        strcpy(buf.text, line);
        if (msgsnd(qid, &buf, sizeof(buf) - sizeof(buf.msgtype),
                   IPC_NOWAIT) == -1) {
            perror("msgsnd (message)");
            error_code = 1;
            free(line);
            break;
        }
        free(line);
    }

    // End the local receiver, then clean up the queue as in the skeleton.
    if (kill(pid, SIGINT) == -1) perror("kill");
    if (waitpid(pid, NULL, 0) == -1) perror("waitpid");
    if (msgctl(qid, IPC_RMID, NULL) == -1 && errno != EIDRM && errno != EINVAL) {
        perror("msgctl (IPC_RMID)");
        error_code = 1;
    }
    return error_code;
}

void get_input(char **buf, size_t *size, char *prompt, ...) {
    va_list args;
    va_start(args, prompt);
    if (vfprintf(stdout, prompt, args) < 0) {
        va_end(args);
        error_code = 1;
        should_exit = 1;
        return;
    }
    va_end(args);
    fflush(stdout);

    errno = 0;
    if (getline(buf, size, stdin) == -1) {
        if (errno != 0 && !(errno == EINTR && should_exit)) {
            perror("getline");
            error_code = 1;
        }
        should_exit = 1;
    }
}

int get_id(char *text) {
    char *input = NULL;
    size_t len = 0;
    get_input(&input, &len, text);
    if (should_exit) {
        free(input);
        return 0;
    }

    char *end;
    errno = 0;
    long value = strtol(input, &end, 10);
    if (errno != 0 || end == input || (*end != '\n' && *end != '\0') ||
        value <= 0 || value >= (long)ACK_TYPE) {
        fprintf(stderr, "Invalid user ID.\n");
        should_exit = 1;
        error_code = 1;
        free(input);
        return 0;
    }
    free(input);
    return (int)value;
}

void sigint_handler(int sig) {
    if (sig == SIGINT) should_exit = 1;
}

void setup_handler(void) {
    struct sigaction action = {0};
    action.sa_handler = sigint_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (sigaction(SIGINT, &action, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
}
