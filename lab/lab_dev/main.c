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

static const char *empty_string = "";

void print_info(const char *name, time_t start_time)
{
    struct tm *local_time;
    local_time = localtime(&start_time);

    printf("Thread name:\t%s\n"
           "PID:\t\t\t%d\n"
           "TID:\t\t\t%lu\n"
           "Start time: \t%s\n",
           name, getpid(),
           pthread_self(),
           asctime(local_time));
}

void *thread_func1(void *name)
{
    pid_t pid;
    posix_spawn_file_actions_t fileActions;
    posix_spawnattr_t spawnattr;
    time_t start_time = time(NULL);

    assert(name != NULL);

/*    if (posix_spawn_file_actions_init(&fileActions)) {
        handle_error("posix_spawn_file_actions_init()");
    }

    if (posix_spawnattr_init(&spawnattr)) {
        posix_spawn_file_actions_destroy(&fileActions);
        handle_error("posix_spawnattr_init()");
    }

    if (posix_spawnp(&pid, "ls", &fileActions, &spawnattr, (char *const *) &empty_string,
                     (char *const *) &empty_string)) {
        posix_spawn_file_actions_destroy(&fileActions);
        posix_spawnattr_destroy(&spawnattr);
        handle_error("posix_spawnp()");
    }*/

    pid = fork();
    if (!pid) {
        print_info(name, start_time);
    } else if (pid < 0) {
        handle_error("fork()");
    }

    print_info(name, start_time);

/*    posix_spawn_file_actions_destroy(&fileActions);
    posix_spawnattr_destroy(&spawnattr);*/

    pthread_exit(0);
}

void *thread_func2(void *name)
{
    time_t start_time = time(NULL);

    assert(name != NULL);

    print_info(name, start_time);

    pthread_exit(0);
}

main()
{
    pthread_t ch1, ch2;

    pthread_create(&ch1, NULL, thread_func1, "Thread1");
    pthread_create(&ch2, NULL, thread_func2, "Thread2");

    pthread_join(ch1, 0);
    pthread_join(ch2, 0);
}
