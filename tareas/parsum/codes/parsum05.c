#include<stdio.h>

int valor_de_secuencia(int pos);
char secuencia[100];//solo acepta datos de 100 caracteres

int cont;//representa la posicion del caracter de la secuencia ingresada
int suma=0;//es la suma de los numeros
int num;//el el numero de parentesis anidados

int main()
{   int cont=0;
    scanf("%s",secuencia);
    do
    {
    num=0;
    num=valor_de_secuencia(cont);
    suma=suma+num;
    cont=cont+num*2;
    }while(secuencia[cont]!='\0');
    printf("%d",suma);
    return 0;
}

int valor_de_secuencia(int pos)
{
    if(secuencia[pos]=='\0')
    {
        return num;
    }
    if(secuencia[pos]=='(')
    {
        if(secuencia[pos+1]==')')
        {
            num=num+1;
            return valor_de_secuencia(pos+1);
        }
        else
        {
        return valor_de_secuencia(pos+1);
        }
    }
    if(secuencia[pos]==')')
    {
        if(secuencia[pos+1]==')')
        {
            num=num+1;
            return valor_de_secuencia(pos+1);
        }
        else
        {
            return num;
        }
    }
}
