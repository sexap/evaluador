#include <stdio.h>

void hanoi(int, int, int, int);
void mueve(int, int);

int main(void)
{
   int n, a=1, b=2, c=3;
   scanf("%d",&n);
   if(n>=1 && n<=20){
      hanoi(n,a,c,b);
      printf("%d %d\n",0,0);
   }
   system("pause");
   return 0;
}

void hanoi(int n, int origen, int temporal, int destino)
{
   if(n>0){
      hanoi(n-1, origen,temporal,destino);
      mueve(origen,destino);
      hanoi(n-1, temporal, origen,destino);
      mueve(destino,temporal);
      hanoi(n-1,origen,temporal,destino);
   }
}

void mueve(int origen, int destino)
{
   printf("%d %d\n",origen,destino);
}
