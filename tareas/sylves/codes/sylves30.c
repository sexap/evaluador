/* Montiel Luna Emmanuel
 * Matricula: 208331279
 * Dise??o de Algoritmos
 * 12-I
 */

#include <stdio.h>

long int sylvester(long int, long int);
int main(){
	long int resultado;
	long int n,m;
	scanf("%ld",&n);
	scanf("%ld",&m);
	resultado=sylvester(n,m);
	printf("%ld\n",resultado);
	return 0;
}
long int sylvester(long int n, long int m){
	long int parcial;
	if(n==0)
		return 2;
	parcial=sylvester(n-1,m)% m;
	return (parcial*(parcial-1)+1) % m;
}
