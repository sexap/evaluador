/* File:   main.c
 * Author: lolita
 * Created on 29 de enero de 2012, 12:12 PM */

#include <stdio.h>

int M,N;
unsigned long int r;

unsigned long int syl(int n);

int main() {
    scanf("%d",&N);
    scanf("%d",&M);
    r=syl(N);
    printf("%d\n",r%M);

    return 0;
}
unsigned long int syl(int n)
{   int t=0;
    if (n==0)
        {return 2;}
    else
          { t=syl(n-1);
            return ((t*(t-1))+1);}
}
