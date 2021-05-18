#include <stdio.h>
#include <stdlib.h>
#include <time.h>

main()
{
    size_t i;

    srand(time(NULL) + 1905);

    for (i = 0; i < 10; ++i)
    {
        printf("%d ", (rand() % 99) + 1);
    }
}
