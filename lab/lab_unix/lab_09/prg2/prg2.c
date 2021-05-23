#include <assert.h>
#include <fcntl.h>
#include <semaphore.h>
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "libs/libs.h"


#define SEM_INITIAL         1
#define SEM_SHARED          1
#define PARENT_STRING       "THIS IS PARENT; PROCESS ID="
#define THREAD_STRING       "This is child thread tid="


static const char *const child_argv[] = { "/home/anton/C/lab/lab_unix/lab_09/prg2/client", NULL };
static const char *const child_argv_synced[] = { "/home/anton/C/lab/lab_unix/lab_09/prg2/client", "-s", NULL };
static char *const *child_argv_to_use = (char *const *) child_argv;


static int init_child_env(int argc, const char **argv, sem_t **sem, pthread_mutex_t **mutex)
{
    int shm_fd = 0;

    assert(argv != NULL);
    assert(sem != NULL);

    if ((argc == 2) && ((!strcasecmp(*(argv + 1), "--sync")) || (!strcasecmp(*(argv + 1), "-s")))) {
        child_argv_to_use = (char *const *) child_argv_synced;

        if ((shm_fd = shm_open(IPC_NAME, O_RDWR | O_CREAT, 0644)) < 0) {
            handle_error("shm_open()");
        }

        if (ftruncate(shm_fd, sizeof(sem_t))) {
            handle_error("ftruncate()");
        }

        if ((*sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0)) == (void *) -1) {
            handle_error("mmap()");
        }
        assert(*sem != NULL);

        if (sem_init(*sem, SEM_SHARED, SEM_INITIAL)) {
            handle_error("sem_init()");
        }

        if (pthread_mutex_init(*mutex, NULL)) {
            handle_error("pthread_mutex_init()");
        }
    } else {
        *sem = NULL;
        *mutex = NULL;
    }

    return shm_fd;
}


static void *thread_func(void *_mutex)
{
    pthread_mutex_t *mutex;
    char buf[BUFSIZ] = {};

    sprintf(buf, "%s%lu\n", THREAD_STRING, pthread_self());

    mutex = _mutex;
    if (mutex != NULL) {
        print_messy_string_synced_mutex(buf, -1, mutex);
    } else {
        print_messy_string(buf, -1);
    }

    pthread_exit(0);
}


int main(int argc, const char *argv[])
{
    pthread_t thrd_array[3];
    pthread_mutex_t mutex, *pmutex = &mutex;
    char buf[BUFSIZ] = {};
    pid_t pid_arr[3];
    sem_t *sem;
    int shm_fd;
    size_t i;

    srand(time(NULL) * 2);

    shm_fd = init_child_env(argc, argv, &sem, &pmutex);

    for (i = 0; i < sizeof(pid_arr) / sizeof(pid_arr[0]); ++i) {
        if (posix_spawn(pid_arr + i, child_argv_to_use[0], NULL, NULL, child_argv_to_use, NULL)) {
            handle_error("posix_spawn()");
        }
    }

    sprintf(buf, "%s%d\n", PARENT_STRING, getpid());

    if (sem != NULL) {
        print_messy_string_synced_sem(buf, -1, sem);
    } else {
        print_messy_string(buf, -1);
    }

    for (i = 0; i < sizeof(pid_arr) / sizeof(pid_arr[0]); ++i) {
        if (waitpid(*(pid_arr + i), NULL, WUNTRACED) < 0) {
            handle_error("waitpid()");
        }
    }

    if (sem != NULL) {
        munmap(sem, sizeof(sem_t));
        close(shm_fd);
        shm_unlink(IPC_NAME);
    }

    write(1, "\n", 1);

    for (i = 0; i < sizeof(thrd_array) / sizeof(thrd_array[0]); ++i) {
        if (pthread_create(thrd_array + i, NULL, thread_func, pmutex)) {
            handle_error("pthread()");
        }
    }

    for (i = 0; i < sizeof(thrd_array) / sizeof(thrd_array[0]); ++i) {
        if (pthread_join(*(thrd_array + i), 0)) {
            handle_error("pthread_join()");
        }
    }

    if (pmutex != NULL) {
        if (pthread_mutex_destroy(pmutex)) {
            handle_error("pthread_mutex_destroy()");
        }
    }

    return 0;
}
