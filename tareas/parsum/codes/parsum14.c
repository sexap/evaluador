#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Monroy Salazar Hayde??
//205203558

#define N 150

int main()
{
	char cadena1[N];
	int  i,a=0;
	scanf("%s",cadena1);	
	int tam = strlen(cadena1);	
	for (i=0; i<tam;i++)
	{
		if(cadena1[i]=='(' || cadena1[i]==')' )
		a= a+1;
		  
		else
		printf("");
		
		
		
	}
	printf("%d \n",a/2);
	return 0;
}
