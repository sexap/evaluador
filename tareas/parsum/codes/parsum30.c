/* Montiel Luna Emmanuel
 * Matricula: 208331279
 * Dise??o de Algoritmos
 * 12-I
 */
#include <stdio.h>
int main(){
	char letra;
	int resultado;
	resultado=0;
	while((letra=getchar())!='\n'){
		if(letra=='(')
			resultado++;
		}
	printf("%d\n",resultado);
	return 0;
}
