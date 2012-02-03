#include <iostream>
using namespace std;
int cont=0,s,i,bajo,alto,**mat;
int enmedio(int ,int);

int min(int k, int j)
{
    if(j<k)
        return j;
    return k;

}

int enmedio(int n, int m)
{  
    if(n>0)
      {
        s=0;
        for(i=m;i>0;i--)
        {
        
         if( i>=bajo || i<=alto )
         {
          cont++;
          s= s+ enmedio(n-i,min(i,n-i));
          }
        }
        return mat[n][m]=s;  
    }
    return mat[n][m]=1;
}

int main()
{
    int n,sum=0;
	cin>>n>>bajo>>alto;
	
	mat = new int*[n];
   
   //crea matriz dinamica n x n
   for(int i=0;i<=n;i++)
     mat[i]= new int [n];
     
     //poner matriz en -1
    for(int i=0;i<=n;i++)
      for(int j=0;j<=n;j++)
          mat[i][j]=-1;
	
   cout<< enmedio(n,n);
	
    cout<<endl;
}
