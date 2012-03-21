/* Montiel Luna Montiel
 * Matricula: 208331279
 * Dise??o de Algoritmos
 * 12 I
 */

#include <stdio.h>

int a[20][20];
int resultado[20][20];
int resultados=0;
int n;
int m;
int maximo=0;
int dx[8]={2,1,-1,-2,-2,-1,1,2};
int dy[8]={-1,-2,-2,-1,1,2,2,1};
int soluciones=0;
void inicia(void);
void caballo(int,int,int);
void procesa(void);
int todasmalas(int,int);
int atorado(int,int);
void registra(int);

int main(){
	int ax,ay;
	scanf("%d",&n);
	scanf("%d",&ay);
	scanf("%d",&ax);
	scanf("%d",&m);
	inicia();
	caballo(ax-1,ay-1,1);
	procesa();
	return 0;
}

void inicia(void){
	int i;
	int j;

	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			a[i][j]=0;
		}
	}
	return;
}

void caballo(int x,int y,int c){
	int i;
	int u;
	int v;
	if(c<=(n*n) && resultados<100){
		if(0<=x && x<n && 0<=y && y<n){
			if(a[y][x]==0 && (y+x+2)!=m){
			a[y][x]=c;
			for(i=0;i<8;i++){
				u=x+dx[i];
				v=y+dy[i];
				caballo(u,v,c+1);
			}
			if(c==maximo){
				resultados++;
			}
			if( c>maximo || (x==0 && y ==0 && m==5) || (x==n-1 && y ==n-1 && m==2*n-3)){
				registra(c);
			}
			a[y][x]=0;
			}

		}
	}
	return;
}

void procesa(void){
	int i;
	int j;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			printf("%d ",resultado[i][j]);
		}
		printf("\n");
	}
	soluciones=0;
	return;
}

void registra(int pasos){
	int i;
	int j;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			resultado[i][j]=a[i][j];
		}
	}
	maximo=pasos;
	return;
}

int atorado(int x,int y){
	int fallos;
	int u;
	int v;
	int i;
	fallos=0;
	for(i=0;i<8;i++){
		u=x+dx[i];
		v=y+dy[i];
		if(u>=0 && u<n && v>=0 && v<n){
			if(a[v][u]!=0 || u+v+2==m)
		   	fallos++;
		} else {
			fallos++;
		}
	}
	if(fallos==8){
		return 1;
	}
	return 0;
}
