#include <stdio.h>

char par;
int i=0;

int main(){
 
 do{
  
  par = getchar();
   if(par=='(')
    i++;
  }while(par!='\n');

 printf(" %d",i);
 return 0;
}
