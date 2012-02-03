#include <stdio.h>

int sumasi(int n, int p, int q)
{
	int cont = 0, s;
	if (n <= 0)
	{
		if(n == 0)
			return 1;
		return 0;
	}
	for (s = p; s <= q; s++)
		cont+=sumasi(n - s, s+1, q < (n - s) ? q : n - s);
	return cont;
}

int main(void)
{
	int n, p, q;
	scanf("%d%d%d", &n, &p, &q);
	printf("%d\n", sumasi(n, p, q));
	return 0;
}
