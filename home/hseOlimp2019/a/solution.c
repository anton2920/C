#include <stdbool.h>
#include <stdio.h>


inline bool isOdd(long num)
{
	return (num & 1) == 1;
}


inline bool isEven(long num)
{
	return !isOdd(num);
}


int main(void)
{
	long i = 1, nitems;
	long currentNumber;

	scanf("%ld", &nitems);
	scanf("%ld", &currentNumber);

	if (isEven(currentNumber)) {
		for (; i < nitems; ++i) {
			scanf("%ld", &currentNumber);

			if (isEven(currentNumber)) {
				printf("x");
			} else {
				printf("+");
				++i;
				break;
			}
		}
	}
	for (; i < nitems; ++i) {
		scanf("%ld", &currentNumber);

		if (isOdd(currentNumber)) {
			printf("x");
		} else {
			printf("+");
			++i;
			break;
		}
	}
	for (; i < nitems; ++i) {
		printf("x");
	}
	printf("\n");
}
