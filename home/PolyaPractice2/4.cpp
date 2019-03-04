#define _CRT_SECURE_NO_WARGINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string>

#define sizemass 100

void pere(int mass[], int lenght);
int check(int num_element, int mass_comp[], int lenght);
void randfull(int mass[], int lenght);
void userfull(int mass[], int lenght);
int check_mass(int mass[], int lenght);

int main()
{
	setlocale(0, "rus");
	int massive[sizemass], lenght, how, massiveres[sizemass];
	printf("������� 4.\n������������ ������ ��������� �������.\n");
	do
	{
		printf("\n������� ���������� ��������� � �������(2+): "); scanf_s("%d", &lenght);
		if (lenght < 2 || lenght>100)
			printf("\n\t��������! �� ����� ������������ �������� ��� ������� �������. \n\n\t����������: ����������� � ������� ��� �� �����������, �.�. ��������� ������ �����-���� �����.\n\t���������� ��� ���...\n");
	} while (lenght < 2 || lenght>100);

	do
	{
		printf("\n��� ��������� ������? (1 - ������, 2 - �����): "); scanf_s("%d", &how);
		if (how < 1 || how > 2)
			printf("\n\t��������! �� ����� ������������ �������� ��� ������� ����������. \n\t ���������� ��� ���...\n");
	} while (how < 1 || how > 2);

	switch (how)
	{
	case 1: randfull(massiveres, lenght); break;
	case 2: userfull(massiveres, lenght); break;
	}

	printf("\n\n\t ������: \n\n");
	for (int i = 0; i < lenght; i++)
	{
		printf(" %d ", massiveres[i]);
	}
	pere(massive, lenght);

	printf("\n\n\t��������� ����� ��������������:\n\n");
	for (int i = 0; i < lenght; i++)
	{
		printf(" %d ", massiveres[massive[i]]);
	}
	printf("\n\n\n");
	system("pause");
	return 0;
}



// ����������
void pere(int mass[], int lenght)
{
	int temp, compare_mass[sizemass];
	memset(compare_mass, -111111, sizeof(compare_mass));
	for (int i = 0; i < lenght; i++)
	{
		srand(NULL);
		while (true)
		{
			if ((temp = rand() % lenght) != i && check(temp, compare_mass, lenght))
			{
				mass[i] = temp;
				compare_mass[i] = temp;
				break;
			}
		}
	}
}


// �������� �� ���������� ���������� ��� ��������� ���������
int check(int num_element, int mass_comp[], int lenght)
{
	for (int i = 0; i < lenght; i++)
	{
		if (num_element == mass_comp[i])
			return 0;
	}
	return 1;
}

//���������� ����. ������� �������
void randfull(int mass[], int lenght)
{
	for (int i = 0; i < lenght; i++)
	{
		mass[i] = rand() % 100 + 1;
	}
}

void userfull(int mass[], int lenght)
{
	while (true)
	{
		for (int i = 0; i < lenght; i++)
		{
			printf("������� %d-� ������� �������: ", i); scanf_s("%d", &mass[i]);
		}
		if (check_mass(mass, lenght) == 0)
		{
		
			printf("\n\t��������! ����������� ��� � ����� ���������� �������� �������. \n\t���������� ��� ���...\n\n");
			continue;
		}
		break;
	}
}

int check_mass(int mass[], int lenght)
{
	for (int i = 0; i < lenght; i++)
	{
		if (mass[i] == mass[i + 1])
			return 0;
	}
	return 1;
}