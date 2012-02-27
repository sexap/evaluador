#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main ()
{
    char nombre[100];
	//printf( "Escriba cadena: " );
    scanf( "%s", nombre );
	//char cachito[100];
	int x,a,b;
	a=0;
	b=0;	
	for(x=0;x<100;x++)
	{
		if(nombre[x] == 40)
		{
			//printf("Si es\n");
			a=a+1;
		}else 
		  {
			if(nombre[x]== 41)
			{
		  		b=b+1;		
			}
		}
	}
	if(a==b)
	{
		printf("%d",a);
	}
	return 0;
}
