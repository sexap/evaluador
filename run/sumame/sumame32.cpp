#include <iostream>
using namespace std;
int Q,N;
int memoizacion[1002][1002];
int nfk(int p,int sub)
{
	if(memoizacion[p][sub] != -1)
		return memoizacion[p][sub];
	if(sub==N)
		return memoizacion[p][sub] = 1;	
	else
	{
		int cuantas=0;
		for(int i=p;i<=Q&&sub+i<=N;i++)
			cuantas += nfk(i+1,sub+i);
		return memoizacion[p][sub] = cuantas;
	
	}
}
int main()
{
	int p;
	cin>>N>>p>>Q;
	for(int i=0;i<=Q+1;i++)
	{
		for(int j=0;j<=N;j++)
		{
			memoizacion[i][j] = -1;
		
		}
	}
	cout<<nfk(p,0)<<endl;
	return 0;
}
