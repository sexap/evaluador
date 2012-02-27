/*
    Serrano Lopez Jose Luis (208206686)
    Diseno de Algoritmos Tarea 2

    hanoim mueve n discos del pilar a al pilar b, el pilar c sobra
    imprime el pilar de origen y destino cada que realiza un movimiento
 */

#include<stdio.h>

void hanoim (int n, int a, int b, int c) {
  if (n==1) {
    if (c!=2) {
      printf("%d %d\n",a,b);
    } else {
      printf("%d %d\n",a,c);
      printf("%d %d\n",c,b);
    }
  } else {
    if (c!=2) {
      hanoim(n-1,a,c,b);
      printf("%d %d\n",a,b);
      hanoim(n-1,c,b,a);
    } else {
      hanoim(n-1,a,c,b);
      hanoim(n-1,c,b,a);
      printf("%d %d\n",a,c);
      hanoim(n-1,b,c,a);
      hanoim(n-1,c,a,b);
      printf("%d %d\n",c,b);
      hanoim(n-1,a,c,b);
      hanoim(n-1,c,b,a);
    }
  }
}

int main (void) {
  int n;
  scanf("%d",&n);
  hanoim(n,1,3,2);
  printf("0 0");
  return 0;
}
