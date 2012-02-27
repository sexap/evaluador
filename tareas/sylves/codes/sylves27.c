#include<stdio.h>

int sylvester(int );
int main ()
{
  int N,M;
  scanf("%i %i",&N,&M);
  printf("%i\n",sylvester(N)%M);

  return 0;
}

int sylvester(int N)
{
    int a,b=0;
    if (N==0) return 2;
    else
    {
        a=sylvester(N-1);
        b=a-1;
    return (a*b)+1;
    }
}
