#include<stdio.h>
int main (void) {
  char entrada[1048576];
  long i, n=0;
  fflush(stdin);
  fgets(entrada,1048575,stdin);
  for(i=0;i<1048576&&entrada[i]!='\n';i++) if (entrada[i]=='(') n++;
  printf("%ld",n);
  return 0;
}
