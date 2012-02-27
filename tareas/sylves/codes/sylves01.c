/*Celene Dorali Alfaro Quintero
Matricula: 208301193*/

#include<stdio.h>
#include<string.h>

int Sylvester(int n){
double s1=0;
if(n==0)
  return 2;
s1=Sylvester(n-1);
return s1*(Sylvester(n-1)-1)+1;
}

int main(){
char NM[100],*ptr;
int N=0,M=0;
  fgets(NM,100,stdin);
  ptr=strtok(NM," ");
  N=atoi(ptr);
  ptr=strtok(NULL," ");
  M=atoi(ptr);
  printf("\n%d",Sylvester(N)%M);
return 0;
}
