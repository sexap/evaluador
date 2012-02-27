#include <stdio.h>
#include <stdlib.h>

//Monroy Salazar Hayde??
//205203558

int sylves(int n);


int main()
{
	int i,x,res;
	scanf("%d", &i);
	scanf("%d", &x);
	res=sylves(i)%x;
	printf("%d \n",res);
	//printf("%d",aux);
	return 0;
}

int sylves(int n)
{
	//aux++;
	if(n==0) return 2;
	//else if (n==1) return 1;
	else return (sylves (n-1)*(sylves(n-1)-1)+1);
}
