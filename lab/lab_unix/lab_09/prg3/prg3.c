#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define handle_error(_msg)  \
    do {                    \
        perror(_msg);       \
        exit(EXIT_FAILURE); \
    } while (0);

#define NT  10

static pthread_cond_t conds[NT - 1];
static pthread_mutex_t mutex;

_Noreturn static void *thread_func(void *_thread_num)
{
    size_t *thread_num = _thread_num;
    assert(thread_num != NULL);

    pthread_mutex_lock(&mutex);
    pthread_cond_wait(conds + *thread_num, &mutex);

    for (;;) {
        pthread_mutex_lock(&mutex);

        printf("%lu\n", *thread_num);
        sleep(1);

        pthread_cond_signal(conds + ((*thread_num + 1) % (NT - 1)));
        pthread_cond_wait(conds + *thread_num, &mutex);
    }
}


main()
{
    pthread_t thrd_array[NT - 1];
    size_t thrd_indices[NT - 1];
    size_t i;

    for (i = 0; i < sizeof(thrd_indices) / sizeof(thrd_indices[0]); ++i) {
        thrd_indices[i] = i;
    }

    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < sizeof(conds) / sizeof(conds[0]); ++i) {
        if (pthread_cond_init(conds + i, NULL)) {
            handle_error("pthread_cond_init()");
        }
    }

    for (i = 0; i < sizeof(thrd_array) / sizeof(thrd_array[0]); ++i) {
        if (pthread_create(thrd_array + i, NULL, thread_func, thrd_indices + i)) {
            handle_error("pthread_create()");
        }
    }

    pthread_cond_signal(&conds[thrd_indices[0]]);

    for (i = 0; i < sizeof(thrd_array) / sizeof(thrd_array[0]); ++i) {
        if (pthread_join(*(thrd_array + i), 0)) {
            handle_error("pthread_join()");
        }
    }

    return 0;
}
