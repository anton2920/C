#include <assert.h>
#include <gmp.h>
#include <omp.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>


#define P_MAX_BOUND 82589932


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


bool is_definitely_prime(mpz_ptr num)
{
    bool result = true;
    mpz_t bound, i;
    int first_bit;

    assert(num != NULL);

    if (mpz_cmp_ui(num, 1) == 0) {
        return false;
    } else if (mpz_cmp_ui(num, 2) == 0) {
        return true;
    } else {
        first_bit = mpz_tstbit(num, 0);
        if (first_bit == 0) {
            return false;
        }
    }

    mpz_init(bound);

    mpz_sqrt(bound, num);
    for (mpz_init_set_ui(i, 3); mpz_cmp(i, bound) <= 0; mpz_add_ui(i, i, 2)) {
        if (mpz_divisible_p(num, i)) {
            result = false;
            break;
        }
    }

    mpz_clear(bound);
    mpz_clear(i);

    return result;
}


bool is_prime(mpz_ptr num)
{
    assert(num != NULL);

    /* Use probabilistic test first */
    switch (mpz_probab_prime_p(num, 25)) {
        case 2:
            return true;
        case 1:
            return true;
            /* Algorithm is not sure about this number, check the old way */
            /*is_definitely_prime(num);*/
        case 0:
            return false;
        default:
            break;
    }
}


void sighandler(int signo)
{
    exit(0);
}


int main()
{
    struct timeval start_time;
    unsigned long p; /* exponent for Mersenne prime */

    signal(SIGTERM, sighandler);
    signal(SIGINT, sighandler);

    gettimeofday(&start_time, NULL);

#pragma omp parallel for num_threads(8), schedule(static, 1), default(none), shared(start_time)
    for (p = 2; p < P_MAX_BOUND; ++p) {
        mpz_ptr num = get_mersenne(p);
        assert(num != NULL);

        if (is_prime(num)) {
            struct timeval end_time;
            mpz_ptr perfect_number = get_perfect(p);
            assert(perfect_number != NULL);

            gettimeofday(&end_time, NULL);
#pragma omp critical
            {
                gmp_printf("[TID=%d][P=%lu][PN=%Zd] Time is %lf\n", omp_get_thread_num(), p, perfect_number,
                           (end_time.tv_sec + (double) end_time.tv_usec / 1000000) -
                           (start_time.tv_sec + (double) start_time.tv_usec / 1000000));
            }

            mpz_clear(perfect_number);
            free(perfect_number);
        }
        mpz_clear(num);
        free(num);
    }

    return 0;
}
