#include <stdio.h>
#include <stdlib.h>
int S(int N);
    
    
int main()
{
  int N,M;
  int resultado,salida;  
  scanf("%d",&N);
  scanf("%d",&M);
  
  if (N>= 0 && M<=1000){
     resultado=S(N);
     salida=resultado%M;
     printf("%d",salida);
  } 
  system("pause"); 
  return(0);
  
}
int S(int N)
{
  if (N==0){
     return(2);
  }else{
     return( S(N-1)*(S(N-1)-1)+1 );
  }       
}
          
