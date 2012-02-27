#include<stdio.h>
#include<stdlib.h>

void hanoi(int n,int a,int b,int c);

int main()
{
    int N;
    scanf("%d",&N);
    //system("clear");
    hanoi(N,1,2,3);
    //printf("N=%d",N);
    printf("0 0");
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
