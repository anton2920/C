#pragma warning (disable : 4996)
#include<iostream>
#include<ctime>

void turn_user(int *coin)
{
	int choice, choice_1, choice_2, kol,kol_pair=0;
	for (int i = 0; i < 24; i++)
	{
		if ((coin[i] == 1) && (coin[i + 1] == 1)) kol_pair++;
	}
	for (int i = 0; i < 25; i++)
	{
		printf("%d(%d) ", coin[i], i);
	}
	printf("\n");
	do
	{
		printf("������� ����� �� ������ �����?\n");
		scanf("%d", &kol);
	} while (((kol > 2) || (kol < 1))||((kol == 2) && (kol_pair == 0)));
	if (kol == 1)//������������ ����� 1 ������
	{
		do
		{
			printf("�������� ����� ������ �����\n");
			scanf("%d", &choice);
		} while (coin[choice] == 0);
		for (int i = 0; i < 25; i++)
		{
			coin[choice] = 0;
		}
	}
	else//������������ ����� 2 ������
	{
		do
		{
			printf("�������� ����� ������ �����\n");
			scanf("%d", &choice_1);
			scanf("%d", &choice_2);
		} while ((coin[choice_1] == 0) || (coin[choice_2] == 0) || (abs(choice_1 - choice_2) != 1));
		for (int i = 0; i < 25; i++)
		{
			coin[choice_1] = 0;
			coin[choice_2] = 0;
		}
	}
}
void turn_computer(int *coin)
{
	int choice, choice_1, choice_2, kol,kol_pair=0;
	for (int i = 0; i < 24; i++)
	{
		if ((coin[i] == 1) && (coin[i + 1] == 1)) kol_pair++;
	}
	do
	{
		kol = rand() % 2 + 1;
	} while (((kol > 2) || (kol < 1)) || ((kol == 2) && (kol_pair == 0)));
	if (kol == 1)//��������� ����� 1 ������
	{
		do
		{
			choice = rand() % 25;
		} while (coin[choice] == 0);
		for (int i = 0; i < 25; i++)
		{
			coin[choice] = 0;
		}
	}
	else//��������� ����� 2 ������
	{
		do
		{
			choice_1 = rand() % 25;
			choice_2 = rand() % 25;
		} while ((coin[choice_1] == 0) || (coin[choice_2] == 0) || (abs(choice_1 - choice_2) != 1));
		for (int i = 0; i < 25; i++)
		{
			coin[choice_1] = 0;
			coin[choice_2] = 0;
		}
	}
}
int summa(int *coin)
{
	int sum=0;
	for (int i = 0; i < 25; i++)
	{
		sum += coin[i];
	}
	return sum;
}
int main()
{
	setlocale(LC_ALL, "russian");

	int coin[25] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };
	int first;
	srand(time(NULL));
	first = rand() % 2 ;
	printf("������� ����:\n������ ��� ����� ���������� ��� �����, ���� 1, �� ������ ����� �����, ���� 0, �� �� ������ �������.\n�� ��� ����� ����� ���� ��� ��� ����� ������� ������.\n����� ����� ������ ��� ������ ����� ������ �� ��� �� ������, ������� ������� � �������.\n����������� ��� ���� ������ �����.\n����, ��� ������ ����� ���������� ������\n");

	if (first == 0)//������ ����� ������������ 
	{
		printf("�� ������ ������\n");
		do
		{
			turn_user(coin);
			if (summa(coin) == 0)
			{
				printf("�� ��������!!!\n");
			}
			turn_computer(coin);
			if (summa(coin) == 0)
			{
				printf("�� ��������(\n");
			}
		} while (summa(coin) > 0);
	}
	else//������ ����� ���������
	{
		printf("������ ����� ���������\n");
		do
		{
			turn_computer(coin);
			if (summa(coin) == 0)
			{
				printf("�� ��������(\n");
				break;
			}
			turn_user(coin);
			if (summa(coin) == 0)
			{
				printf("�� ��������!!!\n");
				break;
			}
		} while (summa(coin) > 0);
	}
	system("pause");
	return 0;
}