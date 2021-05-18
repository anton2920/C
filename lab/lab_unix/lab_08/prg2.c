#include <stdio.h>
#include <pthread.h>
#include <assert.h>

void *thread_func(void *name)
{
    assert(name != NULL);

    printf("Name of the thread: %s\n", (char *) name);

    pthread_exit(0);
}

main()
{
    pthread_t ch1, ch2;

    pthread_create(&ch1, NULL, thread_func, "Thread1");
    pthread_create(&ch2, NULL, thread_func, "Thread2");

    pthread_join(ch1, 0);
    pthread_join(ch2, 0);
}