// invers07.c
//David Gonzalez Pineda
//208302474

#include <stdio.h>
#include <stdlib.h>
unsigned int inver(unsigned int n,unsigned int pos,int *pa)
{
	int k=0;
	if (pos<n)
	{
		int g=0;
		for(g=pos+1;g<=n;g++)
		{
			if (pa[pos]>pa[g]){k++;}
		}
		return (k + inver(n,++pos,pa));
	}
	return 0;
}

int main()
{
	unsigned int N=0,k=0,nbytes=0,i=0;
	int * pa=NULL;
	scanf("%u",&N);
	nbytes = N * sizeof(int);
	if((pa = (int *)malloc(nbytes))==NULL)
	{
	printf("Insuficiente espacio en memoria\n");
	exit(-1);
	}
	for(i=0;i<=N;i++)
	{
		pa[i]=0;
	}
	for(k=1;k<=N;k++)
	{
		scanf("%d",&pa[k]);
	}
	unsigned int resultado=0;
	resultado=inver(N,1, pa);
	printf("%u\n",resultado);
	return 0;
}


