#include <stdio.h>
int s(int N);

int main(){
int m,n,l;
scanf("%d %d",&n,&m);
//printf("%d %d",n,m);
//l=(int);
printf("%i\n",(s(n)%m));
return 0;
}

int s(int N){
int z=0;
if (N==0)return 2;
z=s(N-1);
return z*(z-1)+1; 
}