#include <assert.h>
#include <pthread.h>
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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

#define TIMEBUF_SIZE 1024

void print_time(const char *supplement)
{
    char timebuf[TIMEBUF_SIZE] = {};
    struct tm *lt;
    time_t t;

    assert(supplement != NULL);

    t = time(NULL);
    lt = localtime(&t);
    strftime(timebuf, TIMEBUF_SIZE, "%T", lt);

    printf("%s:\t\t%s\n", supplement, timebuf);
}

void print_info(const char *name)
{
    printf("Thread name:\t%s\n"
           "TID:\t\t\t%lu\n",
           name, pthread_self());
}

void print_dead(void)
{
    printf("Thread is dead!\n");
}

void print_pid(void)
{
    printf("PID:\t\t\t%d\n", getpid());
}

void *thread_func(void *name)
{
    assert(name != NULL);

    print_time("Start time");
    print_info(name);
    print_pid();
    print_time("End time");
    print_dead();

    pthread_exit(0);
}

main()
{
    pthread_t ch1, ch2;
    pid_t child_pid;

    child_pid = fork();

    if (!child_pid) {
        system("pidin");

        print_pid();
        print_time("End time");
        print_dead();
    } else {
        if (pthread_create(&ch1, NULL, thread_func, "Thread1")) {
            handle_error("pthread_create()");
        }
        if (pthread_create(&ch2, NULL, thread_func, "Thread2")) {
            handle_error("pthread_create()");
        }

        pthread_join(ch1, 0);
        pthread_join(ch2, 0);

        print_pid();
        print_time("End time");
        print_dead();
    }
}
