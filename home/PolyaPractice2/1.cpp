#pragma warning(disable : 4996)

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#define N 100



int main()
{
    setlocale(LC_ALL, "rus");
	int i, a, b, l;
    int n , m = 0, t = 0;
	int arr[N] = { 0 };
	int arr1[N] = { 0 };
	int s[N] = { 0 };
	int q[N] = { 0 };

    do
    {
    printf("1)������������ ������ �� ���� ��������\n");
    printf("2)������������ ������ �� �������(�� �����)\n");
    scanf("%d", &a);
    switch (a)
    {
        case 1:
        {
            do
            {
				printf("������� ����������� ������� �������: ");
				scanf("%d", &n);
            } while(n < 1 || n > 100);

            printf("������� �������� ������� �������:\n");
            for(i = 0; i < n; i++)
            {
                scanf("%d", &arr[i]);
				s[m++] = arr[i];
            }

            do
            {
				printf("������� ����������� ������� �������: ");
				scanf("%d", &l);
            }
            while(l < 1 || l > 100);

            printf("������� �������� ������� �������:\n");
            for(i = 0; i < l; i++)
            {
                scanf("%d", &arr1[i]);
				s[m++] = arr1[i];
            }

            printf("������������ ������: ");
            for(i = 0; i < l + n; i++)
                printf("%d ", s[i]);
        }break;
        case 2:
        {
            do
            {
                printf("������� ����������� ������� �������: ");
                scanf("%d", &n);
            }
            while(n<1 || n>100);
            printf("������� �������� ������� �������:\n");
            for(i = 0; i < n; i++)
            {
                scanf("%d", &arr[i]);
                q[t++]=arr[i];
            }
            do
            {
                printf("������� ����������� ������� �������: ");
                scanf("%d", &l);
            }
            while(l<1 || l>100);
            printf("������� �������� ������� �������:\n");
            for(i = 0; i < l; i++)
            {
                scanf("%d", &arr1[i]);
                s[m++]=arr1[i];
            }
            do
            {
            printf("�� ������ ������� ������� ��������?\n");
            printf("1 ��� 2?\n");
            scanf("%d", &b);
            switch (b)
            {
                case 1:
                {
                    for(i = 0; i < n; i++)
                        printf("%d ", arr[i]);
				}break;
                case 2:
                {
                    for(i = 0; i < l; i++)
                        printf("%d ", arr1[i]);
                }break;
            }
            }while(b < 1 || b > 2);
        }break;
    }
    }while(a < 1 || a > 2);

	system("pause");
	return 0;
}
