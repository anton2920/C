#include <stdio.h>
#include <stdlib.h>
#include <locale>
#include <time.h>

int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "Rus");
	int chislo = 0, diff, k, i;
	printf("�������: ���� ���������� � ����� 0. �� ��� ����� ���������� � ���������� ����� ����� ����� �� 1 �� 10. ���������� ���������� ����� 100.\n");

	do
	{
		do
		{
			k = 0;
			printf("������������ ������ ����� = ");
			scanf_s("%d", &diff);
			if ((diff > 10) || (diff < 1))
			{
				k = 1;
				printf("������������ �����. ������� ����� ��� ���.\n");
			}
			else
			{
				chislo += diff;
				printf("������������ ����� = %d\n", chislo);
			}
			i = 0;
		} while (k == 1);	

		if (chislo < 90)
		{
			diff = 1 + rand() % (10);
			printf("��������� ������ ����� = %d\n", diff);
			chislo += diff;
			printf("������������ ����� = %d\n", chislo);
		}
		else
		{
			if (chislo < 100)
			{
				diff = 100 - chislo;
				printf("��������� ������ ����� = %d\n", diff);
				chislo += diff;
				printf("������������ ����� ����� = %d\n", chislo);
				i = 1;
			}
		}
	} while (chislo < 100);

	if (chislo == 100)
	{
		if (i == 0)
		{
			printf("����������! �� ��������\n");
		}
		else
		{
			printf("�� ���������. ��������� �������\n");
		}
	}
	else
	{
		printf("�� ��������� (�������� ������� ����)\n");
	}

	system("pause");
	return 0;
}