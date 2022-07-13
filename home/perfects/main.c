#include <assert.h>
#include <gmp.h>
#include <omp.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>


#define P_MAX_BOUND 82589932


static bool exitted = false;


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
    mpz_t bound, i, tmp;
    bool result = true;

    assert(num != NULL);

    if (mpz_cmp_ui(num, 1) == 0) {
        return false;
    } else if (mpz_cmp_ui(num, 2) == 0) {
        return true;
    } else {
        mpz_init(tmp);
        mpz_mod_ui(tmp, num, 2);
        if (mpz_cmp_ui(tmp, 0) == 0) {
            mpz_clear(tmp);
            return false;
        }
        mpz_clear(tmp);
    }

    mpz_init(tmp);
    mpz_init(bound);

    mpz_sqrt(bound, num);
    for (mpz_init_set_ui(i, 3); (mpz_cmp(i, bound) <= 0) && (!exitted); mpz_add_ui(i, i, 2)) {
        mpz_mod(tmp, num, i);
        if (mpz_cmp_ui(tmp, 0) == 0) {
            result = false;
            break;
        }
    }

    mpz_clear(bound);
    mpz_clear(tmp);
    mpz_clear(i);

    return (!exitted) ? result : false;
}


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

#pragma omp parallel for num_threads(4), schedule(static, 1), default(none), shared(exitted, start_time)
    for (p = 2; p < P_MAX_BOUND; ++p) {
        mpz_ptr num = get_mersenne(p);
        assert(num != NULL);

        if (is_prime(num)) {
            struct timeval end_time;
            mpz_ptr perfect_number = get_perfect(p);
            assert(perfect_number != NULL);

            gettimeofday(&end_time, NULL);
#pragma omp critical
            gmp_printf("[TID=%d][P=%lu][PN=%Zd] Time is %lf\n", omp_get_thread_num(), p, perfect_number,
                       (end_time.tv_sec + (double) end_time.tv_usec / 1000000) -
                       (start_time.tv_sec + (double) start_time.tv_usec / 1000000));

            mpz_clear(perfect_number);
            free(perfect_number);
        }
        mpz_clear(num);
        free(num);

        if (exitted) {
            p = P_MAX_BOUND;
        }
    }

    return 0;
}
