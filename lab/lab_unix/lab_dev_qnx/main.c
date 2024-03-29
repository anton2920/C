#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <spawn.h>
#include <signal.h>
#include <unistd.h>


#define handle_error(_msg)  \
    do {                    \
        perror(_msg);       \
        exit(EXIT_FAILURE); \
    } while (0)


int main()
{
    size_t i;

    for (i = 0; i < 1000; ++i) {
        usleep(1 * 1000);
    }

    return 0;
}
