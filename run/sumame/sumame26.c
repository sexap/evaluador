/*
    ==> Memoizacion
	Calcule la cantidad S de formas en que se puede 
	obtener el valor N sumando enteros distintos entre 
	P y Q. Dos sumas se consideran id??nticas si solo 
	difieren en el orden de sus sumandos.
	La entrada a tu programa seran los tres enteros N, P y Q 
	(con 1 <= P <= Q <= N <= 1000) y la salida de tu programa sera el entero S.
*/
/*
  Nombre: Jesus Quiroz Dominguez
  Matricula: 204200955

*/
jajajaja
#include <time.h>
#include <stdio.h>
#define MAXVAL 100
int r, s;
int sumasi( int , int ) ;
int A[MAXVAL];

int main(){
  int i, N;
  scanf("%d", &N);

  scanf("%d", &r);
  scanf("%d", &s);

  if( r < 1 || r > s) return -1;
  if( s < r || s > N) return -1;
  if( N < s || N > MAXVAL) return -1;

  for(i=0; i<N; i++)
    A[i]=0;
  printf("%d", msumasi(N, N));

}
/**********************************/
int msumasi(int n, int k) {
   int i, j = 0;
   if( n > 0 ) {
      if( k >= n && A[n] != 0) return A[n];
      for(i = 1; i <= k && i <= n; i++) {
          if(r <= i && i <= s)
            j += msumasi(n-i, i-1);
      if( k >= n ) A[n] = j;
      }
   }
   else return 1;
   return j;
}
