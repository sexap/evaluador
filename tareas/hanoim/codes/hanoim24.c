/*  File:   main.c
 * Author: lolita
 * Created on 4 de febrero de 2012, 03:25 PM
 */
#include <stdio.h>
void hai(int n, int a, int b, int c);

int main() {
    int n;
    scanf("%d",&n);
    hai(n,1,2,3);
    printf("%d %d \n",0, 0 );
    return 0;
}
void hai(int n, int a, int b, int c)
{ if (n==1)
    { printf("%d %d \n",a, b);
      printf("%d %d \n",b, c);
    }
 else
    { hai(n-1,a,b,c);
      printf("%d %d \n",a, b );
      hai(n-1,c,b,a);
      printf("%d %d \n",b, c );
      hai (n-1, a, b,c);
    }
return;
}
