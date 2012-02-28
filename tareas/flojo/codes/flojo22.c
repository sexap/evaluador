/* 
    Serrano Lopez Jose Luis (208206686)
    Diseno de Algoritmos Tarea 3

    flojo intenta poner el caballo c en la casilla x,y de un tablero de nxn,
    mantiene el registro del menor numero de saltos
 */
#include<stdio.h>

int dx[]={2,1,-1,-2,-2,-1,1,2};
int dy[]={-1,-2,-2,-1,1,2,2,1};
int tablero[20][20];
int tcorto[20][20];
int saltos=1000;
int n,n2;

int flojo (int x, int y, int c){
  int jumps,i,j;
  jumps=0;
  if (c<=n2){
    if (-1<x && x<n && -1<y && y<n && tablero[x][y]==0){
      if (c<saltos){
        tablero[x][y]=c;
        for(i=0;i<8;i++)
          jumps+=flojo(x+dx[i],y+dy[i],c+1);
        if (jumps==0){
          for(i=0;i<n;i++)
            for(j=0;j<n;j++)
              tcorto[i][j]=tablero[i][j];
          saltos=c;
        }
        tablero[x][y]=0;
      }
      return 1;
    }
  }
  return 0;
}

int main (void){
  int x,y,i,j;
  scanf("%d %d %d",&n,&x,&y);
  n2=n*n;
  for(i=0;i<n;i++)
    for(j=0;j<n;j++)
      tablero[i][j]=0;
  flojo(x-1,y-1,1);
  for(i=0;i<n;i++){
    for(j=0;j<n;j++){
      printf("%d",tcorto[i][j]);
      if (j<n-1)
        printf(" ");
    }
    if (i<n-1)
      printf("\n");
  }
  return 0;
}
