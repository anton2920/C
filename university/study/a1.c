#include <stdio.h>
#include <locale.h>

int main() {
	float a, b, c;

	setlocale (0, "");

	scanf("%f", &a);
	scanf("%f", &b);

	c = 0.5 * a * b;

	printf("c = %4.2f\n", c);

	return 0;
}
