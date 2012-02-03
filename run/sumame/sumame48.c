#include <time.h>
#include <stdio.h>
#define MAXVAL 100
int p, q;
int sumasi( int n, int k) ;
int V[MAXVAL];

int msumasi(int n, int k) {
   int i, j = 0;
   if( n > 0 ) {
      if( k >= n && V[n] != 0) return V[n];
      for(i = 1; i <= k && i <= n; i++) {
          if(p <= i && i <= q)
            j += msumasi(n-i, i-1);
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
  if( n < q || n > MAXVAL) return -1;

  for(i=0; i<n; i++)
    V[i]=0;
  printf("%d", msumasi(n, n));

}


