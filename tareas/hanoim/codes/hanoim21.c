#include <stdio.h>
int aux=0;
void hanoi(int n,int a,int b,int c)
{
if (n==1)
	{
		printf("%d %d\n",a,b);

		if (aux==0)
		{
			printf("%d %d\n",b,c);
			aux++;
		}
	}
else
	{
		hanoi(n-1,a,b,c);
		printf("%d %d\n",a,b);
		hanoi(n-1,c,a,b);
		printf("%d %d\n",b,c);
		hanoi(n-1,a,b,c);
		hanoi(n-1,b,c,a);
	}
return;
}
int main()
{
	int n,a=1,b=2,c=3;
		//printf("escribe el numero de discos: ");
		scanf("%d",&n);
if(n>=1 && n<=20)
		hanoi(n,a,b,c);
	printf("0 0\n");
return 0;
}
