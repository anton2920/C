typedef struct {
    unsigned int    gp_offset;
    unsigned int    fp_offset;
    void            *overflow_arg_area;
    void            *reg_save_area;
} a_list[1];


int n_sum(int n, ...)
{
    int sum = 0;
    a_list ap;

    /* a_start() */
    ap->gp_offset = 8;
    ap->fp_offset = 48;
    __asm__ __volatile__ ("leaq 16(%%rbp), %%rax\n\t"
                          "movq %%rax, %0\n\t"
                          "leaq -176(%%rbp), %%rax\n\t"
                          "movq %%rax, %1\n\t"
                          : "=m" (ap->overflow_arg_area), "=m" (ap->reg_save_area)
                          :
                          : "%rax");

    while (n--) {
        /* a_arg() */
        void *argp;
        if (ap->gp_offset <= 47) {
            argp = ap->reg_save_area + ap->gp_offset;
            ap->gp_offset += sizeof(long);
        } else {
            argp = ap->overflow_arg_area;
            ap->overflow_arg_area += sizeof(long); /* FIXME */
        }
        sum += (int) *(long *) argp;
    }

    return sum;
}


#include <stdio.h>
#include <stdarg.h>


int n_sum_proper(int n, ...)
{
    int sum = 0;
    va_list ap;

    va_start(ap, n);
    while (n--) {
        sum += va_arg(ap, int);
    }

    return sum;
}


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *range_extraction(const int *args, size_t n)
{
    char *range_str = NULL;
    char numbuf[20];
    size_t i, j;

    assert(args != NULL);

    range_str = malloc(BUFSIZ);
    assert(range_str != NULL);

    *range_str = '\0';

    for (i = 0; i < n; i = j) {
        for (j = i + 1; (j < n) && (args[j] - args[j - 1] == 1); ++j)
            ;
        snprintf(numbuf, sizeof(numbuf), "%d", args[i]);
        strcat(range_str, numbuf);
        if ((j - i) > 2) {
            strcat(range_str, "-");
            snprintf(numbuf, sizeof(numbuf), "%d", args[j - 1]);
            strcat(range_str, numbuf);
        }
        strcat(range_str, ",");
    }

    range_str[strlen(range_str) - 1] = '\0';

    return range_str;
}


main()
{
    printf("Range string: %s\n", range_extraction((const int[]) { -6, -3, -2, -1, 0, 1, 3, 4, 5, 7, 8, 9, 10, 11, 14, 15, 17, 18, 19, 20 }, 20));
}
