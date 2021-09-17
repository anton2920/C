#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int N = 20;

void readm(int *a, int n) {

    int i;

    for (i = 0; i < n; ++i) {
        scanf("%d", &a[i]);
    }
}

void writem(int *a, int n) {

    int i;

    for (i = 0; i < n; ++i) {
        printf("%d ", a[i]);
    }
}

void randm(int *a, int n) {

    int i;

    int a1 = 5, b1 = 15;
    for (i = 0; i < n; ++i) {
        a[i] = (rand() % (b1 - a1 + 1)) + a1;
    }
}

void filem(int *a, int n) {

    FILE *f;
    f = fopen("1.txt", "r");

    int i;

    for (i = 0; i < n; ++i) {
        fscanf(f, "%d", &a[i]);
    }
    fclose(f);
}

int min(int *a, int n) {

    int m = a[0], i;
    for (i = 1; i < n; ++i) {
        if (*(a + i) < m) {
            m = a[i];
        }
    }

    return m;
}


int main() {

    srand(time(NULL));

    int a[N];
    int n, v;

    scanf("%d", &n);

    scanf("%d", &v);

    switch (v) {
        case 1:
            readm(a, n);
            break;
        case 2:
            randm(a, n);
            break;
        case 3:
            filem(a, n);
            break;
    }

    writem(a, n);
    printf("\n");
    printf("%d", min(a, n));


    return 0;
}