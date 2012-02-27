// hanoim07.c 
//David Gonzalez Pineda
//208302474

#include <stdio.h>

unsigned int mueve(unsigned int _1, unsigned int _2)
{
	printf("%u %u\n",_1,_2);
	return 0;
}

unsigned int hanoim(unsigned int n,unsigned int T1,unsigned int T2,unsigned int T3)
{
	if (n>0)
	{
		hanoim(n-1,T1,T2,T3);
		mueve(T1,T2);
		hanoim(n-1,T3,T2,T1);
		mueve(T2,T3);
		hanoim(n-1,T1,T2,T3);
	}
	
	return 0;
}


int main()
{
	unsigned int N=1;

	scanf("%u",&N);
	hanoim(N,1,2,3);
	printf("0 0\n");
	return 0;
}
