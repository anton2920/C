#include <stdio.h>

main(){

    int a, b, flag;
    b = 0;
    flag = 0;

    while (1) {
        printf("Type number: ");
        while (1) {
            a = getchar();
            if (a >= '0' && a <= '9') {
                b = (b * 10) + (a - '0');
                flag = 1;
            } else if (a != '\n') {
                flag = 0;
            }
            if (a == '\n') {
                break;
            }
        }
        if (flag == 1) {
            break;
        }
    }

    printf("Number = %d\n", b);
}