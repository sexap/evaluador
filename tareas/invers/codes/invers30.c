/* Montiel Luna Emmanuel
 * Matricula: 208331279
 * Dise??o de Algoritmos
 * 12I
 * Problema 2: Sea N un entero positivo y sea A1, A2, ..., AN un vector de N enteros positivos distintos.
 * Diremos que dos ??ndices I y J forman una inversi??n si I < J pero AI > AJ.
 * Dise??a un algoritmo de divide y vencer??s que calcule la cantidad de inversiones en el vector A.
 */
#include <stdio.h>
#define MM 1000000

int datos[MM];
int inversiones;
int mergesort(int ,int );

int main(){
	int i;
	int n;
	inversiones=0;
	scanf("%d",&n);
	for(i=0;i<n;i++){
		scanf("%d",&datos[i]);
	}
	/*	printf("%d\n",i);*/
	inversiones=mergesort(0,n-1);
	printf("%d\n",inversiones);
	return 0;
}
int mergesort(int inf,int sup){
	int mit;
	int a;
	int b;
	if(sup<=inf)
		return 0;
	else{
		mit=(sup+inf)/2;
		
		a=mergesort(inf,mit);
		b=mergesort(mit+1,sup);
	return merge(inf,mit,sup)+a+b;
	}

}

int merge(int a,int b,int c){
	int aux [1000000];
	int i, j, k,inver;
	i=a;
	j=b+1;
	k=a;
	inver=0;
	while((i<=b)&&(j<=c))
	{
		if(datos[i]<datos[j])
		{
			aux[k]=datos[i];
			k++;
			i++;
		}
		else
		{
			aux[k]=datos[j];
			k++;
			inver+=(b-i)+1;
			j++;
		}
	}
	while(i<=b)
	{
		aux[k]=datos[i];
		k++;
		i++;
	}
	while(j<=c)
	{
		aux[k]=datos[j];
		k++;
		j++;
	}
	for(i=a;i<k;i++)
	{
		datos[i]=aux[i];
	}
	return inver;

}
