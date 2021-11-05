#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <spawn.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>


#define handle_error(_msg)  \
    do {                    \
        perror(_msg);       \
        exit(EXIT_FAILURE); \
    } while (0)

pthread_mutex_t mutex;
timer_t timer;

int payload;

void *__attribute__ ((noreturn)) producer(void *__attribute__ ((unused)) _data)
{
    size_t count;

    for (count = 0; 1; ++count) {
        if (count == 7) {

        }
        timer_create(CLOCK_REALTIME, )

        pthread_mutex_lock(&mutex);
        payload = rand() % 1000;
        pthread_mutex_unlock(&mutex);
    }
}

void *__attribute__ ((noreturn)) consumer(void *__attribute__ ((unused)) _data)
{
    struct timespec possible_delay;

    for (;;) {
        if (pthread_mutex_timedlock(&mutex, &possible_delay) == ETIMEDOUT) {
            printf("I had enough! Aborting now!");
            abort();
        }
        printf("Data received: %d\n", payload);
        pthread_mutex_lock(&mutex);
    }
}


int main()
{
    pthread_t prod_thrd, cons_thrd;

    if (timer_create(CLOCK_REALTIME, NULL, &timer) != 0) {
        handle_error("timer_create");
    }

    /*timer_settime(timer, )*/

    if (pthread_mutex_init(&mutex, NULL) != 0) {
        handle_error("pthread_mutex_init");
    }

    if (pthread_create(&prod_thrd, NULL, producer, NULL) != 0) {
        handle_error("pthread_create");
    }
    if (pthread_create(&cons_thrd, NULL, consumer, NULL) != 0) {
        handle_error("pthread_create");
    }

    if (pthread_join(prod_thrd, 0) != 0) {
        handle_error("pthread_join");
    }
    if (pthread_join(cons_thrd, 0) != 0) {
        handle_error("pthread_join");
    }

    if (pthread_mutex_destroy(&mutex) != 0) {
        handle_error("pthread_mutex_destroy");
    }

    return 0;
}
