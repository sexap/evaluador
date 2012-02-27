#include<stdio.h>

int invertir(int a[],int i,int j);
int mezcla(int a[], int ini,int m,int f);

int main(void){

 int n,i;
 scanf("%d\n",&n);
 int e[n];
 for(i=0;i<n;i++){
      scanf("%d",&e[i]);
 }
 printf("%d\n",invertir(e,0,n-1));
 return 0;
}

int invertir(int a[],int i,int j){
   int m,k,l,*b,h=0,r=0,c=0;
   if(i<j){
      m=(i+j)/2;
      r=invertir(a,i,m);
      c=invertir(a,m+1,j);
      h=h+mezcla(a,i,m,j);
   }

   return h+r+c;
}

int mezcla(int a[], int ini,int m,int f){
   int aux[f-ini+1];
   int i=ini;
   int j=m+1;
   int k=0;
   int c=0;
   int n;
    while (i <= m && j <= f) {
        if (a[i] <= a[j]) {
            aux[k] = a[i];
            ++i;
        }
        else {
            aux[k] = a[j];
            c = c + ((m + 1) - i);
            ++j;
        }
        ++k;
    }


    while (i <= m) {
        aux[k] = a[i];
        ++i;
        ++k;
    }

    while (j <= f) {
        aux[k] = a[j];
        ++j;
        ++k;
    }
    for (n = 0; n < (f-ini+1); ++n)
    a[ini + n] = aux[n];
    return c;
}

