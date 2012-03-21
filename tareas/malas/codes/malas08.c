/*Hernandez  Jimenez Raul  206203315 
  Tare 3  Ejercicio 2   */
#include <stdio.h>
#include <stdlib.h>

static int tablero[30][30];
static int dx[8]={ 2, 1,-1,-2,-2,-1,1,2};
static int dy[8]={-1,-2,-2,-1, 1, 2,2,1};

static void inicializacion(int N);
void imprime_tablero(int N);
static int caballo(int c,int x,int y,int N,int M);

int main(void){
    int N,R,C,M;
    N=-1;
    R=-1;
    while(N<0 || N>20){
         scanf("%d",&N);
         if(N>=0 || N<=20){
             break;
         }else{
            //printf("\nValor de N no valido, ingresalo nuevamente!!\n");
         }
    }
    C=N+2;
    while(R<0){
         scanf("%d",&R);       
         if(R>=0){
            break;
         }else{
            //printf("\nValor de R no valido, ingresalo nuevamente!!\n");     
         }      
    }
    while(C>N){
        scanf("%d",&C);
        if(C<=N){
           break;
        }else{
          //printf("\nValor de C no valido, ingresalo nuevamente!!\n");
        }
    }
    M=1;
    R=R+1;
    C=C+1;
    while(M<2 || M>(2*N)){
       scanf("%d",&M);
       if(M>=2 || M<=(2*N)){
           break;
       }else{
          //printf("\nValor de M no valido, ingresalo nuevamente!!\n");
       }   
    }
    inicializacion(N);
    //printf("Solucion:");
    caballo(1,R,C,N,M);
    imprime_tablero(N);
    return 0;
}

static void inicializacion(int N){
    int i,j;
    for(i=0;i<N+4;i++){
        for(j=0;j<N+4;j++){
          if(i>1 && i<N+2 && j>1 && j<N+2){
              tablero[i][j]=0;
          }else{
              tablero[i][j]=-1;
          }
        }
    }
}

void imprime_tablero(int N){
    int i,j;
    for(i=0;i<N+4;i++){
        printf("\n");
        for(j=0;j<N+4;j++){
          if(tablero[i][j]!=-1){
              printf("%4d",tablero[i][j]);
          }
        }
    }
    return;    
}

static int caballo(int c,int R,int C,int N,int M){
    int i,u,v,p,mul;
    tablero[R][C]=c;
    if(c==N*N){
       return 1;
    }else{
       for(i=0;i<8;i++){
          u=R+dx[i];
          v=C+dy[i];
          if(tablero[u][v]==0){
             p=u+v;                  
             if(caballo(c+1,u,v,N,M)){
                mul=p%M;
                if(mul!=0){
                    return 1;
                }
             }else{
                tablero[u][v]=0;
                return 0;
             }
          }
       }
    }
}
