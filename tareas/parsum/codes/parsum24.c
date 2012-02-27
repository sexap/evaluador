/* File:   parsumzz.c
 * Author: lolita
 * Created on 29 de enero de 2012, 10:59 AM  */

#include <stdio.h>
#include <string.h>
char cadena [1001];

int r=0;
int ps(int n);

int main() {
    scanf("%s", cadena);
    r=ps(strlen(cadena));
    printf("%d\n",r);
    return 0;
}


int ps(int n)
{char letra=')';
    if (n==0)
        {return 0;}
    else
    {  if (cadena[n]==letra)
          {return 1+ps(n-1);}
       else
          {return ps(n-1);}
    }
}
