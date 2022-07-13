#include <assert.h>
#include <gmp.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


mpz_ptr get_mersenne(unsigned long p)
{
    mpz_ptr mersenne;

    mersenne = malloc(sizeof(mpz_t));
    assert(mersenne != NULL);

    mpz_init_set_ui(mersenne, 2);
    mpz_pow_ui(mersenne, mersenne, p);
    mpz_sub_ui(mersenne, mersenne, 1);

    return mersenne;
}


mpz_ptr get_perfect(unsigned long p)
{
    mpz_ptr perfect;
    mpz_ptr mersenne;

    perfect = malloc(sizeof(mpz_t));
    assert(perfect != NULL);

    mpz_init_set_ui(perfect, 2);
    mpz_pow_ui(perfect, perfect, p - 1);

    mersenne = get_mersenne(p);
    assert(mersenne != NULL);

    mpz_mul(perfect, perfect, mersenne);

    mpz_clear(mersenne);
    free(mersenne);

    return perfect;
}


bool is_prime(mpz_ptr num)
{
    return true;
}


static bool exitted = false;


void sighandler(int signo)
{
    exitted = true;
}


int main()
{
    struct timeval start_time;
    unsigned long p; /* exponent for Mersenne prime */

    signal(SIGTERM, sighandler);
    signal(SIGINT, sighandler);

    gettimeofday(&start_time, NULL);

    for (p = 2; !exitted; ++p) {
        mpz_ptr num = get_mersenne(p);
        assert(num != NULL);

        if (is_prime(num)) {
            struct timeval end_time;
            mpz_ptr perfect_number = get_perfect(p);
            assert(perfect_number != NULL);

            gettimeofday(&end_time, NULL);
            gmp_printf("[%lu][%Zd] Time is %lf\n", p, perfect_number,
                       (end_time.tv_sec + (double) end_time.tv_usec / 1000000) -
                       (start_time.tv_sec + (double) start_time.tv_usec / 1000000));

            mpz_clear(perfect_number);
            free(perfect_number);
        }
        mpz_clear(num);
        free(num);
    }

    return 0;
}
