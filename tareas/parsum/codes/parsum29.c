//Lucía Echenique Álvarez

#include <stdio.h>
#include <string.h>

int main ()
{
  char str[100];
  int i=0;
  scanf("%s",&str);
  while(str[i]!= '\0')
  {
       i++;
  }
  printf("%d", i/2);
  printf("\n");
  system("PAUSE");
}
