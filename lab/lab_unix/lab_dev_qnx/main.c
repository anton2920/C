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


#define N 3
#define T 500


/* Producer function */
void *producer(void *_tid)
{
    sigset_t mask;
    int signo, num;
    int tid;

    assert(_tid != NULL);

    tid = *((int *) _tid);

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);

    for (;;) {
        usleep(T * 1000);

        /* Random generation */
        num = rand() % 1000;

        printf("Thread with ID #%d generated number: %d\n", tid, num);
        fflush(stdout);

        assert(SIGRTMIN + tid <= SIGRTMAX);
        if (kill(getpid(), SIGRTMIN + tid) < 0) {
            handle_error("kill");
        }
        if (sigwait(&mask, &signo) < 0) {
            handle_error("sigwait");
        }
        if (signo == SIGTERM) {
            break;
        }
    }

    pthread_exit(0);
    return NULL; /* Dummy return to avoid warning */
}


/* Consumer function */
void consumer(void)
{
    int i, signum;
    sigset_t mask;

    sigemptyset(&mask);
    for (i = 0; i < N; ++i) {
        sigaddset(&mask, SIGRTMIN + i);
    }

    for (;;) {
        if (sigwait(&mask, &signum) < 0) {
            handle_error("sigwait");
        }
        if (signum < 0) {
            handle_error("sigwaitinfo");
        }
        if (signum == SIGTERM) {
            break;
        }

        printf("Received signal from #%d\n", signum - SIGRTMIN);
        kill(getpid(), SIGUSR1);
    }
}


int main()
{
    pthread_t producers[N];
    int tid_array[N];
    sigset_t mask;
    size_t i;

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    for (i = 0; i < N; ++i) {
        sigaddset(&mask, SIGRTMIN + i);
    }
    if (sigprocmask(SIG_SETMASK, &mask, NULL) < 0) {
        handle_error("sigprocmask");
    }

    for (i = 0; i < N; ++i) {
        tid_array[i] = (int) i;
        pthread_create(producers + i, NULL, producer, tid_array + i);
    }

    /* Main thread acts as a consumer */
    consumer();

    for (i = 0; i < N; ++i) {
        pthread_join(*(producers + i), 0);
    }

    return 0;
}
