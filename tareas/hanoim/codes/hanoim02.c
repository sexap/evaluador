#include<stdio.h>

void hanoi(int n,int a,int b,int c);

int main()
{
      int n,a,b,c;
      char s;
      
      a=1;
      b=2;
      c=3;
      
      //printf("Dame el valor de n:\n");
      scanf("%d",&n);

      hanoi(n,a,b,c);
      printf("0 0\n");
       
      fflush(stdin);
      scanf("%c",&s);
      
      return 0;
}

void hanoi(int n,int a,int b,int c)
      {
          if(n==1)
          {
          printf("%d %d\n",a,b);
          printf("%d %d\n",b,c);
          }
          else
          {
              hanoi(n-1,a,b,c);
              printf("%d %d\n",a,b);
              hanoi(n-1,c,b,a);
              printf("%d %d\n",b,c);
              hanoi(n-1,a,b,c);
          }
      }
