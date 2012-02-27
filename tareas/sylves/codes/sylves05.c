#include<stdio.h>
int silvester(int dato);

int main()
{   int N;
    int M;
    int num;
    int resp;
    scanf("%d",&N);
    scanf("%d",&M);
    num=silvester(N);
    //printf("el numero es num %d",num);
    resp=num % M;
    //printf("el numero es M %d",N);
    //printf("el numero es resp %d",resp);
    printf("%d",resp);
    return 0;
}

int silvester(int dato)
{
    if(dato==0)
    {
        return 2;
    }
    return (silvester(dato-1))*((silvester(dato-1)) - 1) + 1;
}
