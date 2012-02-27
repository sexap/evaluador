#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//#include <conio.h>

void hanoi(int n,int com, int aux, int fin);

int main(void){

        
	char com='1';
	char aux='2';
	char fin='3';
	int n;

	//printf("\nN??mero de discos: ");
	scanf("%d",&n);
	fflush(stdin);

	//printf("\n\nLos movimientos a realizar son: \n");
	hanoi(n,com,aux,fin);
	
	return 0;
}

void hanoi(int n,int com, int aux, int fin){
 	if(n==1){
            printf("%c %c\n",com,aux);
            printf("%c %c\n",aux,fin);
            }
    else{
         hanoi(n-1,com,aux,fin);
         printf("%c %c\n",com,aux);
         hanoi(n-1,fin,aux,com);
         printf("%c %c\n",aux,fin);
         hanoi(n-1,com,aux,fin);          
         }
}
