#include<stdio.h>

int S(int N);
int main()
{
    int N=0;
    int M=0;
    int X=0;
    scanf("%d",&N);
    scanf("%d",&M);
    X=S(N)%M;
    printf("%d",X);
    return 0;
}

int S(int N)
{
    if(N==0)
    return 2;
    int P=0;
    P=S(N-1);
    return (P*(P-1)+1);   
}
