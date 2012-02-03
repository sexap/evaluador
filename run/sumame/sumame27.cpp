#include<iostream>
using namespace std;
int v[1000]={0};
int F[1000][1000];
int s,i,p,q;
int min(int a, int b)
{
    if(b<=a)
        return b;
    return a;

}
int sumasi(int n, int m)
{
    if(F[n][m] > 0 )
                return F[n][m];
       if(n>0)
       {
            s=0;
            for(i=m;i>0; i--)
            {
                if(i>=p || i<=q )
                {
                    v[i]=1;
                    s= s + sumasi(n-i,min(i,n-i));
                    v[i]=0;
                 }
             }
             return F[n][m]=s;
        }
        else if(n==0)
              return F[n][m]=1;
}

int main()
{
    int n;
    cin>>n>>p>>q;
    cout<<sumasi(n,n)<<endl;
    }
