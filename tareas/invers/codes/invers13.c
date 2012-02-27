#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int main()
{
int n,c,a,i,j,temp,aux,m,ini;
int numeros[1000000];
//printf("numero de elementos:\t");
scanf("%d",&n);
//printf("Da valores de arreglo:\n");
//-------------------------
for(c=1;c<=n;c++)
{
	scanf("%d",&numeros[c]);
}
	if(n==1)
	{
		printf("0");
	}
	else{
		ini=0;		//algoritmo de ordenamiento	
		for(i=1;i<n;i++)
		{			
			for(j=i+1;j<=n-1;j++)
			{
				if(numeros[i]>numeros[j])
				{
					aux=numeros[i];
					numeros[i]=numeros[j];
					numeros[j]=aux;
					ini++;
				}		
			}
		}
		printf("%d\n",ini);		
  		}
  		return 0;
}
