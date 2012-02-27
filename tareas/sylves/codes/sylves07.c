//Elaborado por David Gonzalez Pineda
//Declarcion de Bibliotecas
#include <stdio.h>
unsigned int s(unsigned int n)
{
	unsigned int S=0;
	if (n==0)
	{
		return 2;
	}
	S=s(n-1);
	return S*(S-1)+1;
}

//Programa Principal
int main()
{
	unsigned int N=0,M=0, R=0;
	scanf("%u %u",&N,&M);
	R=s(N);
	printf("%u\n", R%M);
	return 0;
}
