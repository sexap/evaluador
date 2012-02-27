/*Hernandez Jimenez Raul  206203315
    Tarea 2 Ejercicio 1*/
#include<stdio.h>
#include<stdlib.h>
void hanoi(int,char,char,char);

int main(){
    int n=0;
    while(n<1 || n>20){
       //printf("Numero de discos:\n");
       scanf("%d",&n);
       if(n<1 || n>20){
         //     printf("\nNumero de discos incorrecto, ingresalo nuevamente\n");       
       }
    }
    //printf("\nLos movimientos son: \n");
    hanoi(n,'1','2','3');
    printf("0 0\n");
    
    return 0;
}


void hanoi(int n, char a,char b, char c){
     
     if (n>0){
         hanoi(n-1,a,b,c);
         printf("%c %c\n",a,b);
         hanoi(n-1,c,b,a);           
         printf("%c %c\n",b,c);
         hanoi(n-1,a,b,c);
     }
}
