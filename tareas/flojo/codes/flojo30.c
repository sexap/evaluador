/* Montiel Luna Montiel
 * Matricula: 208331279
 * Dise??o de Algoritmos
 * 12 I
 */

#include <stdio.h>

int a[20][20];
int resultado[20][20];
int n;
int dx[8]={2,1,-1,-2,-2,-1,1,2};
int dy[8]={-1,-2,-2,-1,1,2,2,1};
int maximo;
void inicia(void);
void caballo(int,int,int);
void procesa(void);
int atorado(int,int);
void registra(int);

int main(){
	int ax,ay;
	scanf("%d",&n);
	scanf("%d",&ay);
	scanf("%d",&ax);
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
	
	if(n<4){
		maximo=n*n;
	}
	else {
	       	if(n<6){
			maximo=2*n;
		}
		else {
			if(n<17){
				maximo=n;
			}
			else{
				maximo=16;
			}
		}
	}
	maximo++;
	return;
}

void caballo(int x,int y,int c){
	int i;
	int u;
	int v;
	/* Si no se ha alcanzado el maximo de saltos permitidos y no se ha encontrado una solucion */
	if(c<=maximo){
		if(0<=x && x<n && 0<=y && y<n && a[y][x]==0){
			a[y][x]=c;
			for(i=0;i<8;i++){
			u=x+dx[i];
			v=y+dy[i];
			caballo(u,v,c+1);
			}
			if(atorado(x,y) && c<maximo){
				registra(c);
			}
			a[y][x]=0;
		}
	}
	return;
}

void procesa(void){
	int i;
	int j;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			printf("%d",resultado[i][j]);
			if(j<n-1)
				printf(" ");
		}
		printf("\n");
	}
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
			if(a[v][u]!=0)
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
