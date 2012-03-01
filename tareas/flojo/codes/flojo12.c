#include <stdio.h>

int nxn,N;
void flojo(int tablero[][N], int i, int ren, int col);
int C,R;

const int fila[8]={ -2, 2, 2, 1, -2,-1, 1,-1},
       columna[8] = {-1, 1,-1,-2,1, 2, 2,-2};

int main(void)
{
  scanf("%d %d %d",&N,&R,&C);
  int tablero[N][N]; /* tablero del caballo. */
  int i,j;
   nxn=N*N+1;
  for (i = 0; i < N; i++)
   for (j = 0; j < N; j++)
    tablero[i][j] = 0;
  tablero[R-1][C-1] = 1;
  flojo(tablero,2,C-1,R-1);
    for (j = 0; j < N; j++) {
      for (i = 0; i < N; i++)
        printf("%3d ", tablero[j][i]);
      putchar('\n');
    }
//scanf("%d %d %d",&N,&R,&C);
  return 0;
}

void flojo(int tablero[N][N],int i, int ren, int col)
{
  int k, u, v;
  k = 0;
  do {
    u = ren + fila[k]; v = col + columna[k];
    if (u >= 0 && u < N && v >= 0 && v < N)
    {  
      if (tablero[u][v] == 0)
      { 
        tablero[u][v] = i;
        k=8;
      if (i <=nxn)
        {
          flojo(tablero,i+1,u,v);
          }
      }
    }
   k++;
  } while (k<8);
}
   

