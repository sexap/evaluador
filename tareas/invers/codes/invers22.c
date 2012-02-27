/*
    Serrano Lopez Jose Luis (208206686)
    Diseno de Algoritmos Tarea 2

    invers calcula el numero de inversiones presentes en el arreglo a
    entre los indices i y j ademas de regresarlo ordenado
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

long invers (int i, int j, long *a) {
  long inv; /* almacena las inversiones presentes */
  long m,inicio,fin,*b;
  long k,l;
  inv=0;
  inicio=i;
  fin=j;
  if (i<j) {
    m = (i+j)/2;
    inv=invers(i,m,a);
    inv+=invers(m+1,j,a);
    /* combina(i,m,j) */
    b = (long *) calloc (j-i+1,sizeof(long));
    k=m+1;
    l=0;
    while(i<=m && k<=j){
      if (*(a+i)<*(a+k))
        *(b+l)=*(a+i++);
      else {
        *(b+l)=*(a+k++);
        inv+=m-i+1;
      }
      l++;
    }
    while(i<=m){
      *(b+l++)=*(a+i++);
    }
    while(k<=j){
      *(b+l++)=*(a+k++);
    }
    for(i=0;i<fin-inicio+1;i++){
      *(a+inicio+i)=*(b+i);
    }
    free(b);
  }
  return inv;
}

int main (void) {
  long n, *A, m, i;
  scanf("%ld",&n);
  A = (long *) calloc (n,sizeof(long));
  for(i=0;i<n;i++)
    scanf("%ld",A+i);
  m = invers(0,n-1,A);
  printf("%ld",m);
  free(A);
  return 0;
}
