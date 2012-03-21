#include <stdio.h>
int ncuad,N;
void mala(int tablero[][N], int i, int pos_x, int pos_y);
int C,R;
int M,q;

const int ejex[8] = {  1, 2, 2, 1,-1,-2,-2,-1 },
          ejey[8] = {  2, 1,-1,-2,-2,-1, 1, 2};

int main(void)
{
   scanf("%d %d %d %d",&N,&C,&R,&M);
   ncuad=N*N;
  int tablero[N][N];
  int i,j;

  for (i = 0; i < N; i++)
   for (j = 0; j < N; j++)
    tablero[i][j] = 0;

  tablero[C-1][R-1] = 1;
  mala(tablero,2,C-1,R-1);

    for (j = 0; j < N; j++) {
      for (i = 0; i < N; i++)
        printf("%3d ", tablero[i][j]);
      putchar('\n');
    }

  return 0;
}

void mala(int tablero[N][N],int i, int pos_x, int pos_y)
{
  int k, u, v;

  k = 0;
  q=0;
  do {
    u = pos_x + ejex[k]; v = pos_y + ejey[k];
    if (u >= 0 && u < N && v >= 0 && v < N)
    { 
       if (tablero[u][v] == 0)
       {
          if( (((u+2)+(v+2))%(M+1))!=0 )
          { 
             tablero[u][v] = i; 
            if (i <=ncuad+3)
           {  
             mala(tablero,i+1,u,v);
             if (!q)
              {
                tablero[u][v] = 0;
                ncuad--;
               }
            }
             else q=1;
          }
       }      
    }
    k++;

  } while (!q && k<8);
}
   
