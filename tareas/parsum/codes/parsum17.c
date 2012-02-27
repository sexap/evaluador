#include<stdio.h>

int main()
{
	char c[1000];
	int i=0;
	scanf("%s",c);
	for(i=0;c[i]=='(' || c[i]==')';i++){}
	printf("%d",i/2);
	return 0; 
}
