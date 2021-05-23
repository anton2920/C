#include <assert.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

#include "libs/libs.h"

#define CHILD_STRING "this is child; process id="


int main(int argc, const char *argv[])
{
    sem_t *sem;
    int shm_fd;
    char buf[BUFSIZ] = {};

    srand(time(NULL) * 3);

    sprintf(buf, "%s%d\n", CHILD_STRING, getpid());

    if ((argc == 2) && (!strcasecmp(*(argv + 1), "-s"))) {
        if ((shm_fd = shm_open(IPC_NAME, O_RDWR | O_CREAT, 0644)) < 0) {
            handle_error("shm_open()");
        }

        if ((sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0)) == (void *) -1) {
            handle_error("mmap()");
        }
        assert(sem != NULL);

        print_messy_string_synced_sem(buf, -1, sem);
    } else {
        print_messy_string(buf, -1);
    }
}
