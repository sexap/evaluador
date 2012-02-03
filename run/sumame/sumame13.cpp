#include <algorithm>
#include <iostream>
#include <stdio.h>
using namespace std;
int mat[1001][1001];
int p(int n, int m, int p1, int q)
{
    int i=0;
    if(mat[n][m] != -1)
    {
        if(n>0)
        {
            int s=0;
            for(i=1; i<=m; i++)
            {
                if(i>=p1 && i<=q)
                {
					s+=p(n-i,min(i,n-i),p1,i-1);
                }
            }
            mat[n][m]=s;
        }
        else
            return mat[n][m]=1;
    }
    return mat[n][m];
}

int main()
{
    int n, Q, P, suma=0, cont=0;
    scanf("%d %d %d",&n, &P, &Q);

    for(int i=0; i<=n; i++)
        for(int j=0; j<=n; j++)
            mat[i][j]=1;

    cout<<p(n,n,P,Q)<<"\n";
}
