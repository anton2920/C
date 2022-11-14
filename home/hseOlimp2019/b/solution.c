#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


static int numLess(const void *_a, const void *_b)
{
	const int *a = _a, *b = _b;
	if (*a < *b) {
		return -1;
	} else if (*a > *b) {
		return 1;
	} else {
		return 0;
	}
}


int main()
{
	int i, currPos;
	int npeople;
	int fedPosition, minPosition, maxPosition;
	int *patience;

	scanf("%d", &npeople);
	scanf("%d", &fedPosition);

	patience = malloc(sizeof(int) * (npeople - 1));
	assert(patience != NULL);

	for (i = 0; i < npeople - 1; ++i) {
		scanf("%d", patience + i);
	}

	qsort(patience, npeople - 1, sizeof(int), numLess);

	minPosition = fedPosition;
	for (i = 0, currPos = 1; currPos < fedPosition; ++currPos) {
		if (patience[i] < currPos) {
			--minPosition;
			++i;
		}
	}

	maxPosition = fedPosition;
	for (i = npeople - 2, currPos = fedPosition - 1; currPos > 0; --currPos) {
		if (patience[i] < currPos) {
			--maxPosition;
		} else {
			--i;
		}
	}

	printf("%d\n%d\n", minPosition, maxPosition);

	return 0;
}
