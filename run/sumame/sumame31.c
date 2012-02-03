/*Problema 2: Sean N, P y Q tres enteros positivos. Dise??n algoritmo de memoizaci??ue c
alcule la cantidad S de formas en que se puede obtener el valor N sumando enteros distintos
entre P y Q. Dos sumas se consideran id??icas si s??difieren en el orden de sus sumandos. P
or ejemplo, si N = 12, P = 3 y Q = 7 entonces S = 2 porque 12 se puede obtener de las siguientes
formas: 3+4+5 y 5+7.

tarea 5 problema 2 programa sumame31
nombre:israel ruiz ramirez
matricula:204309644
numero de lista:31
*/
#include <time.h>
#include <stdio.h>
#define  TAM 100
int p, q;
int sumame( int n, int k) ;
int V[TAM];

int sumame(int n, int k) {
   int i, j = 0;
   if( n > 0 ) {
      if( k >= n && V[n] != 0) return V[n];
      for(i = 1; i <= k && i <= n; i++) {
          if(p <= i && i <= q)
            j += sumame(n-i, i-1);
      if( k >= n ) V[n] = j;
      }
   }
   else return 1;
   return j;
}

int main(int argc, char* argv[]) {
  int i, n;
  scanf("%d", &n);

  scanf("%d", &p);
  scanf("%d", &q);

  if( p < 1 || p > q) return -1;
  if( q < p || q > n) return -1;
  if( n < q || n > TAM) return -1;

  for(i=0; i<n; i++)
    V[i]=0;
  printf("%d", sumame(n, n));

  printf("\n\n");


}


