#include<stdio.h>

int hanoi(int n,int a,int b,int c);
int main()
{
    int n=0;
    int a=1,b=2,c=3;
    scanf("%d",&n);
    hanoi(n,a,b,c);
    printf("0 0\n");
    return 0;
}

int hanoi(int n,int a,int b,int c)
{
    if(n<2){
            printf("%d %d\n",a,b);
            printf("%d %d\n",b,c);
            }
    else{
         hanoi(n-1,a,b,c);
         printf("%d %d\n",a,b);
         hanoi(n-1,c,b,a);
         printf("%d %d\n",b,c);
         hanoi(n-1,a,b,c);          
         }
         return 0;
}

