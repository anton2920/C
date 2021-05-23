#include <assert.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>


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

#define ITERATION_MAX       10000
#define SEMAPHORE_INITIAL   1

void print_time(const char *supplement, clock_t start_time, clock_t end_time)
{
    assert(supplement != NULL);

    printf("%20s:\t%lu\n", supplement, end_time - start_time);
}

int main()
{
    size_t i;
    clock_t start_time, end_time;
    pthread_mutex_t mutex;
    sem_t unnamed_sem, *named_sem;

    /* Mutex part */
    if (pthread_mutex_init(&mutex, NULL)) {
        handle_error("pthread_mutex_init()");
    }

    start_time = clock();
    for (i = 0; i < ITERATION_MAX; ++i) {
        pthread_mutex_lock(&mutex);
        pthread_mutex_unlock(&mutex);
    }
    end_time = clock();
    print_time("Mutex", start_time, end_time);

    pthread_mutex_destroy(&mutex);

    /* Unnamed semaphore part */
    if (sem_init(&unnamed_sem, 0, SEMAPHORE_INITIAL)) {
        handle_error("sem_init()");
    }

    start_time = clock();
    for (i = 0; i < ITERATION_MAX; ++i) {
        sem_wait(&unnamed_sem);
        sem_post(&unnamed_sem);
    }
    end_time = clock();
    print_time("Unnamed semaphore", start_time, end_time);

    sem_destroy(&unnamed_sem);

    /* Named semaphore */
    named_sem = sem_open("/named_semaphore", O_CREAT, 0644, SEMAPHORE_INITIAL);
    if (named_sem == SEM_FAILED) {
        handle_error("sem_open()");
    }

    start_time = clock();
    for (i = 0; i < ITERATION_MAX; ++i) {
        sem_wait(named_sem);
        sem_post(named_sem);
    }
    end_time = clock();
    print_time("Named semaphore", start_time, end_time);

    sem_close(named_sem);
    sem_destroy(named_sem);

    return 0;
}
