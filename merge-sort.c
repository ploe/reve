#include <stdio.h>
#include <stdlib.h>

void merge(int *a, int n, int m) {
	int i, j, k;
	int *x = calloc(n, sizeof(int));
	for (i = 0, j = m, k = 0; k < n; k++) {
		x[k] =
			(j == n) ? a[i++] :
			(i == m) ? a[j++] :
			(a[j] < a[i]) ? a[j++] :
			a[i++];
	}
	for(i = 0; i < n; i++) {
		a[i] = x[i];
	}
	free(x);
}

void merge_sort(int *a, int n) {
	if (n < 2) return;

	int m = n / 2;

	merge_sort(a, m);
	merge_sort(a + m, n - m);
	merge(a, n, m);
}

int main() {
	int a[] = {4, 65, 2, -31, 0, 99, 2, 63, 782, 1};

	int n = sizeof(a) / sizeof(a[0]);

	int i;
	for(i = 0; i < n; i++) {
		printf("%d, ", a[i]);
	}
	putchar('\n');

	merge_sort(a, n);

	for(i = 0; i < n; i++) {
		printf("%d, ", a[i]);
	}
	putchar('\n');

}
