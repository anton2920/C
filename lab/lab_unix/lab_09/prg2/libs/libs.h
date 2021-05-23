#ifndef LIBS_H
#define LIBS_H

#include <pthread.h>
#include <semaphore.h>

#define handle_error(_msg)  \
    do {                    \
        perror(_msg);       \
        exit(EXIT_FAILURE); \
    } while (0);

#define IPC_NAME    "/Unix09"

void print_messy_string(const char *str, size_t len);
void print_messy_string_synced_sem(const char *str, size_t len, sem_t *sem);
void print_messy_string_synced_mutex(const char *str, size_t len, pthread_mutex_t *mutex);

#endif /* LIBS_H */
