	#include<stdio.h>
int F[1001][1001];
int l[1001];
int sumame(int, int, int, int);
main(){
	int N,P,Q,S;
	scanf("%d %d %d",&N,&P,&Q);
	S=sumame(N,N,P,Q);
	printf("%d", S);
	
}
int sumame(int n, int m,int p, int q){
	int s,i,min;
	if(F[n][m] > 0 ){
		return F[n][m];}
	if(n > 0){
		s=0;
		for(i=m;i>0;i--){
			if(p<=i && i<=q && l[i]==0){
				l[i]=1;
				if(n-i<=i)
					min=n-i;
				else 
					min=i;
				s=s+sumame(n-i,min,p,q);
				l[i]=0;
			}
		}
		return F[n][m]=s;
	}else
		return F[n][m]=1;
}
