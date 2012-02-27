/* Montiel Luna Montiel
 * Matricula: 208331279
 * Dise??o de Algoritmos
 * 12 I
 * Dise??a un algoritmo usando la t??cnica de divide y vencer??s para resolver el problema
 * de las torres de Hanoi con la restricci??n adicional de que no se permite mover ning??n
 * disco entre las agujas 1 y 3.
 */
#include <stdio.h>
void hanoi(int, int,int,int);
int main(){
	int n;
	scanf("%u",&n);
	hanoi(n,1,2,3);
	printf("%d %d\n",0,0);
	return 0;

}
void hanoi(int n, int a,int b,int c){
	if(n==1){
		printf("%d %d\n%d %d\n",a,b,b,c);
	}
	else {
		hanoi(n-1,a,b,c);
		printf("%d %d\n",a,b);
		hanoi(n-1,c,b,a);
		printf("%d %d\n",b,c);
		hanoi(n-1,a,b,c);
	}
	return;
}
