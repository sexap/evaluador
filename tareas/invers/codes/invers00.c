#include <stdio.h>

#define MAX 1000000

int b[MAX];

long long int invers(int a[], int izq, int der)
{
  int i, j, k, m;
  long long int z = 0;

  if (der > izq) {
    m = (der+izq)/2;
    z = invers(a, izq, m);
    z += invers(a, m+1, der);
    for (i = izq; i <= der; i++)
      b[i] = a[i];
    for (i = j = izq, k = m+1; i <= der; i++) {
      if (j > m)
	a[i] = b[k++];
      else if (k > der)
	a[i] = b[j++];
      else {
	if (b[j] < b[k])
	  a[i] = b[j++];
	else {
	  a[i] = b[k++];
	  z += m+1-j;
	}
      }
    }
  }
  return z;
}

int main(void)
{
  int i, n, a[MAX];

  scanf("%d", &n);
  for (i = 0; i < n; i++)
    scanf("%d", &a[i]);
  printf("%lld\n", invers(a, 0, n-1));
  return 0;
}
