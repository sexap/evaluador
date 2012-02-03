/*
Alumno: De la Cruz Torres Armando  Matricula 206241347 Tarea 5*/

#include <math.h>
#include <stdio.h>
# define max 1000

int cont=0;
int n, k;
int P,Q;
int param(int ,int);

int min(int k, int j)
{
    if(j<k)
        return j;
    return k;
}

int mat[max][max];

int param(int n, int m)
{  cont++;
   if(mat[n][m] == -1) 
   { 
      if(n==0) 
         {   
                  printf("\n");
               mat[n][m]=1; 
         } 
      else 
      { 
           int s=0; 
           int i=0;
           for(i=1; i<=m; i++) 
           { if((i>P) && (i<Q))
                   s+=param(n-i,min(i,n-i)); 
                   } mat[n][m] = s; 
           } 
      } return mat[n][m];

}

int main()
{
    int i=0,j=0;
	scanf("%d",&n);
	scanf("%d",&P);
	scanf("%d",&Q);
	if(P>=1)
	{
           if(Q>=P)
           {
             if(n>=Q)
             {
              if(n<=1000)
              {
                mat[n+1][n+1];
	        for(i=0; i<=n;i++)
	         {
                  for (j=0;j<=n ;j++)
                   {
                    mat[i][j]=-1;
                   }
                  }
	          printf(" \t%d\n",param(n,n));
               }
             }
          }
   }
}
