#include <glib.h>
#include <stdio.h>


#define SUFFIX(i) (((i) == 1) ? "st" : ((i) == 2) ? "nd" : ((i) == 3) ? "rd" : "th")


static void _fill_matrix(GArray *matrix, gsize *nrows, gsize *ncols);
static void _find_row_sum(GArray *matrix, gsize nrows, gsize ncols);


main()
{
    gsize nrows, ncols;
    g_autoptr(GArray) matrix;

    matrix = g_array_new(FALSE, FALSE, sizeof(gint));
    g_assert_nonnull(matrix);

    _fill_matrix(matrix, &nrows, &ncols);

    _find_row_sum(matrix, nrows, ncols);
}


static void _fill_matrix(GArray *matrix, gsize *nrows, gsize *ncols)
{
    gint elem;
    gsize i;

    g_assert_nonnull(matrix);
    g_assert_nonnull(nrows);
    g_assert_nonnull(ncols);

    g_print("Type the number of rows: ");
    scanf("%lu", nrows);

    g_print("Type the number of columns: ");
    scanf("%lu", ncols);

    g_print("Now, type elements: \n");

    for (i = 0; i < (*nrows) * (*ncols); ++i) {
        scanf("%d", &elem);
        g_array_append_val(matrix, elem);
    }
}


static void _find_row_sum(GArray *matrix, gsize nrows, gsize ncols)
{
    gsize i, j, sum;

    g_assert_nonnull(matrix);
    g_assert(nrows > 0);
    g_assert(ncols > 0);

    for (i = 0; i < nrows; ++i) {
        sum = 0;
        for (j = 0; j < ncols; ++j) {
            sum += g_array_index(matrix, gint, i * ncols + j);
        }
        g_print("Sum of %lu%s row is %lu\n", i + 1, SUFFIX(i + 1), sum);
    }
}
