/*Autor: Celene Dorali Alfaro Quintero
Matricula:208301193*/

#include <stdio.h>
int A[20][20],Saltosf=400,dx[]={2,1,-1,-2,-2,-1,1,2},dy[]={-1,-2,-2,-1,1,2,2,1},n=0,Mejor[20][20];

int Ocupados(int x,int y){
int k,cont=0;
for(k=0;k<8;k++)
	if(A[x+dx[k]][y+dy[k]]!=0)
	   cont++;
 if(cont==8)
	return 1;
return 0;
}

Caballo(int x,int y,int c,int Saltos){
int i,j;
	if (c<=(n*n)){
		if(1<=x && x<=n && 1<=y && y<=n && A[x][y]==0){
			A[x][y]=c;
			for(i=0;i<8;i++)
				Caballo(x+dx[i],y+dy[i],c+1,Saltos+1);
			if(Ocupados(x,y)==1 && Saltos<Saltosf){
				Saltosf=Saltos;
				for(i=1;i<=n;i++){
					for(j=1;j<=n;j++)
						Mejor[j][i]=A[j][i];
		      }
			}
			A[x][y]=0;
		}
	}
}

Inicializa(int A[20][20]){
int i,j;
	for(i=0;i<20;i++)
	   for(j=0;j<20;j++)
          A[i][j]=1;
	for(i=1;i<=n;i++)
       for(j=1;j<=n;j++)
          A[i][j]=0;
}

int main(){
	int x,i,j,y,Saltos=0;
	scanf("%d %d %d",&n,&y,&x);
    Inicializa(A);
    Inicializa(Mejor);
	Caballo(x,y,1,Saltos+1);
	for(i=1;i<=n;i++){
       printf("\n");
	   for(j=1;j<=n;j++)
          printf("%d ",Mejor[i][j]);
    }
	return 0;
}
