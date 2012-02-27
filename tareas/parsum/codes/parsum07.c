
//David Gonzalez 
//208392474


#include <stdio.h>
#include <stdlib.h>
int par_sum(char c[])
{
	int i=0, j=0, k=0;
	if(c[0]!='(')
	{
		return 0;
	}
	if (c[0]=='(' && c[1]==')')
	{
		return 1+par_sum(c+2);
	}
	for (k=0;(i!=j)||(i==0 && j==0);k++)
	{
		if (c[k]=='(')
		{
			i++;
		}
		if (c[k]==')')
		{
			j++;
		}
	}
	return i+par_sum(c+i+j);

}

int main()
{
	char n[65530];
	int r;
	scanf("%s",&n);
	r=par_sum(n);
	printf("%d\n",r);
	return 0;
}
