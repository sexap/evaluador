/*Hernandez Jimenez Raul
206203315
Tarea 1 - ejercicio 1*/
#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>

int main(void){
    int sum=0,val=0;
    char c;
    
    //printf("Ingrese la cadena de parentesis: ");
    while((scanf("%c",&c))>0 && !isspace(c)){
        if(c=='('){
           val=val+1;        
        }else{
           sum=sum+val;
           val=0; 
        }
    }
    printf("%d\n",sum);
    return 0;
}
