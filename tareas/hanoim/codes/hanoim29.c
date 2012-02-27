#include <stdio.h>

void gira2(int n, int a, int b, int c)
{
   if(n==1)
   {
      printf("%d %d", a, b);
      printf("\n");
      printf("%d %d", b, c);
      printf("\n");
   }     
   else
   {
      gira2(n-1, a, b, c);
      printf("%d %d", a, b);
      printf("\n");
      gira2(n-1, c, b, a);
      printf("%d %d", b, c);
      printf("\n");
      gira2(n-1, a, b, c);
   }
}

int main()
{
   int a=1, b=2, c=3, n=0;
   scanf("%d",&n);
   gira2(n, a, b, c);
   printf("%d %d", 0, 0);
   printf("\n");
   
   return 0;
}
