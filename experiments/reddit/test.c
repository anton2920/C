#include <stdio.h>


void recursion(int num, int perc);


int main(void)
{
    int num;
    int perc = 1;
    printf("Enter a value: ");

    scanf("%d", &num);

    recursion(num, perc);
}

void recursion(int num, int perc)
{
    if (perc <= 100)
        printf("%d%% of the input: %.2f\n", perc, ((num * perc) / 100.00));
    recursion(num, ++perc);
}
