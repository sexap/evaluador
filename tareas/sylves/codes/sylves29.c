//Lucía Echenique Álvarez
//Sylvester

#include <stdio.h>

int sylvester (int n, int m)
{
   if(n==0)
   {
      return(2);
   }    
   else
   {
      return(((sylvester(n-1, m))*(sylvester(n-1, m)-1)+1))% m;
   }
}

int main()
{
   int n, m;
   scanf("%d", &n);
   scanf("%d", &m);
   printf("%d", sylvester(n, m));
   printf("\n");
   system("PAUSE");
}

