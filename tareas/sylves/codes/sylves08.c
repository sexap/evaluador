/*Hernandez Jimenez Raul
206203315
Tarea 1 - ejercicio 2*/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int syl(int);

int main(){
    int n,m,temp,r;

    //printf("valor de N: ");
    scanf("%d",&n);
    r=n;
    //printf("valor de M: ");
    scanf("%d",&m);
    temp=syl(n) % m;
    printf("%d\n",temp);
    return 0;
}

int syl(int n){
    if (n==0){
        return 2;
    }else{
        return syl(n-1)*(syl(n-1)-1)+1;    
    }
}
