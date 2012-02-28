#include <stdio.h>

#define N_CUADROS 8
#define N_SALTOS   8
#define CIERTO 1
#define FALSO  0

static int tablero_virtual[N_CUADROS+4][N_CUADROS+4];
static int dx[N_SALTOS]={ 2, 1,-1,-2,-2,-1, 1, 2};
static int dy[N_SALTOS]={ 1, 2, 2, 1,-1,-2,-2,-1};


void inicia_tablero(void);
void presenta_tablero(void);
void ensaya_movimiento(int n,int x,int y);


int main(void)
{

  inicia_tablero();
  int R,C;
  scanf("%d",&R);
  scanf("%d",&C);
  ensaya_movimiento(1,R,C);
  presenta_tablero();
  return 0;
}

void inicia_tablero(void)
{
  int i,j;
  for(i=0;i<N_CUADROS+4;i++) {
    for(j=0;j<N_CUADROS+4;j++) {
      if(i>1 && i<N_CUADROS+2 && j>1 && j<N_CUADROS+2 )
           tablero_virtual[i][j]=0;
      else
           tablero_virtual[i][j]=-1;
    }
  }

}

void presenta_tablero(void)
{
  int i,j;

  for(i=0;i<N_CUADROS+4;i++) {
     printf("\n");
     for(j=0;j<N_CUADROS+4;j++) {
        if(tablero_virtual[i][j]!=-1)
         printf("%4d",tablero_virtual[i][j]);
     }
  }
  return;
}

void ensaya_movimiento(int n,int x,int y)
{
  int i,u=x,v=y;
  tablero_virtual[x][y]=n;
  if(n < (N_CUADROS*N_CUADROS))
  {
    for(i=0;i<N_SALTOS;i++)
	{
       u=x+dx[i];
       v=y+dy[i];
       if(tablero_virtual[u][v]==0) 
	   {
          ensaya_movimiento(n+1,u,v);
		  break;
       }
    }
  }
} 

