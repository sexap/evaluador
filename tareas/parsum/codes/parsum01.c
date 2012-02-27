/*Celene Dorali Alfaro Quintero
Matricula: 208301193*/

#include<stdio.h>
#include<string.h>

int ab=0;
int par(char cad[100]){
int i; 
char *ptr;
  for(i=0;cad[i]!=')';i++)
       ab++;
  if(i==(strlen(cad)/2))
       return ab;
  ptr=strtok(cad,"");
  return par(ptr+(i*2));
}

int main(){
char p[100];
scanf("%s",&p);
printf("%d\n",par(p));
return 0;
}
