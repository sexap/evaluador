//sumame02.cpp

#include<iostream>
#include<algorithm>
#define maxn 1001
#define maxn2 1000
using namespace std;

int parsumasi(int, int);
int minimo(int, int);

int s,i,p,q;

int comp[maxn2]={0};
int formas[maxn][maxn];


int minimo(int a, int b)
{
    if(b<=a)
    {
        return b;
    }
    else
    {
        return a;
    }

}

int parsumasi(int n, int m)
{
    if(formas[n][m] > 0 )
    {
      return formas[n][m];
    }
    
    if(n>0)
    {
      s=0;
      for(i=m;i>0; i--)
      {
        if(i>=p || i<=q )
        {
           comp[i]=1;
           s= s + parsumasi(n-i,std::min(i,n-i));
           comp[i]=0;
        }
      }

      return formas[n][m]=s;
    }
    
    return formas[n][m]=1;
}

int main()
{
    int n;
    cin>>n;
    cin>>p;
    cin>>q;

    cout<<parsumasi(n,n)<<endl;

//cin.get();
//cin.get();
}
