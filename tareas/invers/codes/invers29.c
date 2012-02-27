#include <stdio.h>

int main()
{
    int k=0, n=0, a[100], i=0, j=0, aux=0, r=0;
    
    scanf("%d",&n);
    for(k=0; k<n; k++)
    {
        scanf("%d",&a[k]);
    }
    for(i=1; i<n; i++)
     {
        for(j=0; j<n-i; j++)
        {
           if(a[j]>a[j+1])
           {
              aux=a[j];
              a[j]=a[j+1];
              a[j+1]=aux;
              r++;
           }
        }
     }
     printf("%d",r);
     printf("\n");
     return 0;
}
