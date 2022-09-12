#include <stdio.h>

int main() {
	int a [100];
	//a ciklus feltöltése
	for (int i = 0; i < 100; i++) {
		a[i] = i;
		printf("%d \n", a[i]);
	}
	printf("################### \n");

	int b[100];
	for (int i = 0; i < 100; i++) {
		b[i] = 999-i;
		printf("%d \n", b[i]);
	}

	printf("################### \n");

	int c[100];
	for (int i = 0; i < 100; i++) {
		c[i] = a[i] + b[i];
		printf("%d \n", c[i]);
	}

	printf("################### \n");



}