/* File:   main.c
 * Author: lolita
 * Created on 29 de enero de 2012, 12:12 PM */

#include <stdio.h>

int N;
int r=0;
int vector[1000000];

void inserta(int n);
void mezmod(int i,int j);
void invierte(int i,int m,int j);

int main() {
    scanf("%d,",&N);
    inserta(N);
    mezmod(1,N);
    printf("%d\n",r);
    return 0;
}
void inserta (int n)
{   int i,val;
    for (i=1; i<=n; i++)
    { scanf("%d,",&val);
      vector[i] = val;
    }
}

void mezmod(int i,int j)
{   int m;
    if (i<j)
    {m=(i+j)/2;
     mezmod(i,m);
     mezmod(m+1,j);
     invierte(i,m,j);
    }
return;
}
void invierte(int i,int m,int j)
{int k =m+1;
    while (i<=m )
   {    while (k<j+1)
        {  if(vector[i] > vector[k])
              {r++;}
           k++;
        }
      k=m+1;

      i++;
   }
return;
}
