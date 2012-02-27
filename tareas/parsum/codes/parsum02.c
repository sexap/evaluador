#include <stdio.h>
#include <string.h>

int main()
{
      int m,n,x,i,j;
      char a[100],caracter,s;
      int inv=0;
      m=0;
      n=0;
      x=0;
     
     //printf("Ingrese secuencia:"); 
     gets(a); 
     j=strlen(a);
     puts(a);
     
     for(i=0;i<j;i++)
     {
            caracter=a[i];
            switch(caracter)
            {
                   case '(':
                        m++;
                        break;
                   case ')':
                        n++;
                        break;
                   default:
                           //printf("caracter invalido");
                           inv=1;
                           i=j;
            }
     }
     if(n==m && inv==0)
     {
             x=x+m;
             printf("%d",x);
     }
     else 
     {
          //if(n!=m)
          //printf(", secuencia incorrecta");
     }
     //scanf("%c",&s);
     return 0;
}
