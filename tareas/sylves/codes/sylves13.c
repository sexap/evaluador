#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int main()
{
int n,m,i,r;
//printf("ingresa N   :");
scanf("%d",&n);
//printf("ingresa M   :");
scanf("%d",&m);
int S_n=2;
	
for(i=0;i<n;i++)
	{
	S_n=(S_n*(S_n-1))+1;	
	}
	r=S_n%m;
	
	printf("%d\n",r);
return 0;	
}

