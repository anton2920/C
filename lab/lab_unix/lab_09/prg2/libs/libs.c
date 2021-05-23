#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "libs.h"

#define SLEEP_MAX           100000
#define SLEEP_MIN           1000


static void print_messy_string_ex(const char *str, size_t len, sem_t *sem, pthread_mutex_t *mutex)
{
    int interval;
    size_t i;

    assert(str != NULL);
    assert(len != 0);

    if (len == -1) {
        len = strlen(str);
    }

    if (sem != NULL) {
        sem_wait(sem);
    }

    if (mutex != NULL) {
        pthread_mutex_lock(mutex);
    }

    for (i = 0; i < len; ++i) {
        write(1, str + i, 1);

        interval = (rand() % SLEEP_MAX - SLEEP_MIN + 1) + SLEEP_MIN;
        usleep(interval); /* Interval is generated in milliseconds */
    }

    if (sem != NULL) {
        sem_post(sem);
    }

    if (mutex != NULL) {
        pthread_mutex_unlock(mutex);
    }
}

void print_messy_string(const char *str, size_t len)
{
    print_messy_string_ex(str, len, NULL, NULL);
}


void print_messy_string_synced_sem(const char *str, size_t len, sem_t *sem)
{
    assert(sem != NULL);

    print_messy_string_ex(str, len, sem, NULL);
}

void print_messy_string_synced_mutex(const char *str, size_t len, pthread_mutex_t *mutex)
{
    assert(mutex != NULL);

    print_messy_string_ex(str, len, NULL, mutex);
}
