 /*  File:   main.c
 * Author: lolita
 * Created on 18 de febrero de 2012, 08:37 AM
 */

#include <stdio.h>
#define MAX 20

int N,Op=0;
int matriz[MAX][MAX];
int maux[MAX][MAX];
int dx[]={2,1,-1,-2,-2,-1,1,2};
int dy[]={-1,-2,-2,-1,1,2,2,1};
void iniciamatriz(int m[MAX][MAX],int n);
void imprimematriz(int m[MAX][MAX],int n);
int cuentam(int m[MAX][MAX],int n); //cuenta ceros
void copiam(int o[MAX][MAX],int d[MAX][MAX],int n); //guarda mejor sol
int caballo(int x, int y, int c);

int main() {

    int x,y;
    scanf("%d,",&N);
    scanf("%d,",&x);
    scanf("%d,",&y);

    iniciamatriz(matriz,N);
    caballo(x,y,1);
    imprimematriz(maux,N);

    return 0;
}



void iniciamatriz(int m[MAX][MAX],int n)
{int i,j;

    for (i=1 ; i<=n ; i++ ) { // Columna i
        for ( j=1 ; j<=n ; j++ ) { // Fila j
         m[i][j]=0;}}
}
void imprimematriz(int m[MAX][MAX],int n)
{int i,j;
for (i=1 ; i<=n ; i++ ) { // Columna i
    for ( j=1 ; j<=n ; j++ ) { // Fila j
    	printf("%c",' '); printf("%d", m[i][j]);}
	printf("\n");
    }
}
int caballo(int x, int y, int c)
{ int i,z,v=0;
if (x>=1 && x<=N && y>=1 && y<=N && matriz[x][y]==0 )
        {matriz[x][y]=c;
         v=1;
            for (i=0; i<8; i++)
            {v+=caballo(x+dx[i],y+dy[i],c+1);}
                if (v==1)//caballo atorado
                { z=cuentam(matriz,N);
                    if(z>Op)
                       { copiam(matriz,maux,N); Op=z;}}
             matriz[x][y]=0;
             return 1;
        }
return 0;
}

int cuentam(int m[MAX][MAX],int n)
{int i,j, s=0;
for (i=1 ; i<=n ; i++ ) { // Columna i
    for ( j=1 ; j<=n ; j++ ) { // Fila j
        if (m[i][j]==0) {s++;}}}
return s;
}
void copiam(int o[MAX][MAX],int d[MAX][MAX],int n) {
int i,j;
for (i=1 ; i<=n ; i++ ) { // Columna i
    for ( j=1 ; j<=n ; j++ ) { // Fila j
     d[i][j]=o[i][j];}}
}

