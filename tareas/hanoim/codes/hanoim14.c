#include <stdio.h>
#include <stdlib.h>

//Monroy Salazar Haydee
//205203558
void hanoi(int n,int com, int aux, int fin);
int main(){
	int com = 1;
	int aux = 2;
	int fin = 3;
	int n;
	scanf("%d",&n);
	fflush(stdin);

	if (n>0){
	hanoi(n,com,aux,fin);
	printf("0 0\n");
	return 0;
	}
	else
	return 0;
}
void hanoi(int n,int com, int aux, int fin){
	if(n==1){
		 printf("%d %d\n",com,aux);
		 printf("%d %d\n",aux,fin);
	}
	else{
		hanoi(n-1,com,aux,fin);
		printf("%d %d\n",com,aux);
		hanoi(n-1,fin,aux,com);
		printf("%d %d\n",aux,fin);
	 	hanoi(n-1,com, aux, fin);
	}


}
