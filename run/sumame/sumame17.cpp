//U.E.A.:DISE?O DE ALGORITMOS
//ALUMNO:MANRIQUE MEJIA CHRISTIAN JOSUE  MATRICULA 206241151
//TAREA 5: SUMAME17

#include <algorithm>
#include <math.h>
#include <iostream>
# define max 1000
using namespace std;
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
         {   //if((cont==Q) && (m==N))
                  cout<<endl;
               mat[n][m]=1; 
         } 
      else 
      { 
           int s=0; 
           for(int i=1; i<=m; i++) 
           { if((i>P) && (i<Q))
                   s+=param(n-i,min(i,n-i)); 
                   } mat[n][m] = s; 
           } 
      } return mat[n][m];

}

int main()
{
    
	cin>>n;
	//N=n;
	cin>>P;
	cin>>Q;
	//cin>>k;
	if(P>=1)
	{
            if(Q>=P)
            {
                    if(n>=Q)
                    {
                            if(n<=1000)
                            {
                                     	mat[n+1][n+1];
	                                    for(int i=0; i<=n;i++)
	                                       {
                                            for (int j=0;j<=n ;j++)
                                             {
                                               mat[i][j]=-1;
                                             }
                                           }
	                                    cout<<param(n,n);
	                                    cin.get();
	                                    cin.get();
                            }
                    }
            }
   }
}
