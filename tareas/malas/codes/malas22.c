/* 
    Serrano Lopez Jose Luis (208206686)
    Diseno de Algoritmos Tarea 3

    malas intenta poner el caballo c en la casilla x,y de un tablero de nxn,
    mantiene el registro del mayor numero de saltos donde (r+c)!=(em) donde e es un natural
 */
#include<stdio.h>

int dx[]={2,1,-1,-2,-2,-1,1,2};
int dy[]={-1,-2,-2,-1,1,2,2,1};
int tablero[20][20];
int tlargo[20][20];
int saltos=0;
int n,n2,m;

int malas (int x, int y, int c){
  int jumps,i,j;
  jumps=0;
  if (c<=n2){
    if (-1<x && x<n && -1<y && y<n && tablero[x][y]==0 && (x+y+2)%m!=0){
      tablero[x][y]=c;
      for(i=0;i<8;i++)
        jumps+=malas(x+dx[i],y+dy[i],c+1);
      if (jumps==0 && c>saltos){
        for(i=0;i<n;i++)
          for(j=0;j<n;j++)
            tlargo[i][j]=tablero[i][j];
        saltos=c;
      }
      tablero[x][y]=0;
      return 1;
    }
  }
  return 0;
}

int main (void){
  int x,y,i,j;
  scanf("%d %d %d %d",&n,&x,&y,&m);
  n2=n*n;
  for(i=0;i<n;i++)
    for(j=0;j<n;j++)
      tablero[i][j]=0;
  malas(x-1,y-1,1);
  for(i=0;i<n;i++){
    for(j=0;j<n;j++){
      printf("%d",tlargo[i][j]);
      if (j<n-1)
        printf(" ");
    }
    if (i<n-1)
      printf("\n");
  }
  return 0;
}
