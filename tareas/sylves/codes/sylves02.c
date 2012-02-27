#include<stdio.h>

int sn(int N);

int main()
{
      int N,M,X;
      char s;
      
      //printf("Dame el valor de N:\n");
      scanf("%d",&N);
      //printf("Dame el valor de M:\n");
      scanf("%d",&M);
      if(M!=0)
          {
          X=sn(N);
          X=X%M;
          printf("%d",X);
          }
      else{
           printf("");}
      fflush(stdin);
      //scanf("%c",&s);
      return 0;
}

int sn(int n)
      {
          if(n==0)
          {
               return 2;
          }
          else
          {
              return (sn(n-1)*(sn(n-1)-1)+1);
          }
      }
