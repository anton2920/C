#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <SDL2/SDL.h>

#define GRAVITY_CONST ((double) 9.8)

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#define CONVERT_DEG_TO_RAD(_degrees) ((_degrees) / 180.0 * M_PI)
#define CONVERT_RAD_TO_DEG(_radians) ((_radians) / M_PI * 180)

#define min(_x, _y) (((_x) < (_y)) ? (_x) : (_y))
#define max(_x, _y) (((_x) > (_y)) ? (_x) : (_y))

#define ERROR_EXIT(fmt, ...)                        \
        do {                                        \
            fprintf(stderr, fmt, ##__VA_ARGS__);    \
            exit(-1);                               \
        } while (0)

main()
{
    /* Initializing variables */
    double b, R, m, L, H, alpha, beta, mu;

    /* I/O flow */
    printf("Type distance from tunnel right side:");
    scanf("%lf", &b);

    printf("Type radius of the ball:");
    scanf("%lf", &R);

    printf("Type mass of the ball:");
    scanf("%lf", &m);

    printf("Type width of the tunnel:");
    scanf("%lf", &L);

    printf("Type height of the tunnel:");
    scanf("%lf", &H);

    printf("Type tunnel angle (alpha):");
    scanf("%lf", &alpha);

    printf("Type ball angle (beta):");
    scanf("%lf", &beta);

    printf("Type friction coefficient: ");
    scanf("%lf", &mu);

    /* Main part */

}