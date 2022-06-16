#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#define handle_error(_msg)  \
    do {                    \
        perror(_msg);       \
        exit(EXIT_FAILURE); \
    } while (0);

#define error_msg(_msg, ...)                \
    do {                                    \
        fprintf(stderr, _msg, __VA_ARGS__); \
        exit(EXIT_FAILURE);                 \
    } while (0);

const char *prompt = "tushino> ";

int get_command(char *buffer, size_t buflen);
void dummy_handler(int signum);

main()
{
    /* Initializing variables */
    int rv, exec_result;
    char command[BUFSIZ];

    signal(SIGINT, dummy_handler);

    /* Main part */
    for ( ;; ) {
        rv = get_command(command, sizeof(command));
        if (rv == EOF || !strcmp(command, "exit")) {
            if (rv == EOF) {
                putchar('\n');
            }
            break;
        } else if (strlen(command)) {
            exec_result = system(command);

            if (exec_result < 0) {
                handle_error("system");
            } else if (WTERMSIG(exec_result) == SIGINT) {
                putchar('\n');
            }
        }
    }
}

int get_command(char *buffer, size_t buflen)
{
    /* Initializing variables */
    char *rv;

    /* I/O flow */
    fputs(prompt, stdout);
    rv = fgets(buffer, buflen, stdin);
    *(buffer + strlen(buffer) - 1) = '\0';

    return (rv == NULL) ? EOF : 0;
}

void dummy_handler(int signum)
{
    /* Main part */
    if (signum == SIGINT) {
        /* FIXME: implement handling for other streams */
        putchar('\n');
        fputs(prompt, stdout);
        fflush(stdout);
    } else {
        error_msg("Signal %d cannot be handled\n", signum);
    }
}
