#include <stdio.h>

void mov(int n, int org, int dest, int aux) {
	if (n == 0) return;
	mov(n-1, org, dest, aux);
	printf("%d %d\n", org, aux);
	mov(n-1, dest, org, aux);
	printf("%d %d\n", aux, dest);
	mov(n-1, org, dest, aux);
}

int main() {
	int discos;
	scanf("%d", &discos);
	mov(discos, 1, 3, 2);
	printf("0 0\n");
	return 0;
}
