/*Hernandez  Jimenez Raul  206203315
  Tarea 2  Ejercicio 2*/
#include<stdio.h>
#include<stdlib.h>
int combina(int v[], int i, int m, int j){
     int k,nes=0;
     k=m+1;
     while(i<=m && k<=j){
         if(i<m && v[i]>v[m]){
            nes++;
         }
         if(k<j && v[k]>v[j]){
            nes++;
         }
         i++; k++;
     }
     i=0; k=m+1;
     while(i<m && k<=j){
         if(v[i]>v[k]){
            nes++;
         }
         i++;k++;
     }
     return nes;
}

int inversiones(int v[],int i, int j){
     int m,ninv=0;
     m=(i+j)/2;
     if (i<j){
         ninv = inversiones(v,i,m);
         ninv = ninv + inversiones(v,m+1,j);
         ninv = ninv + combina(v,i,m,j);
     }
     return ninv;
}
 
int main(){
    int x,n,*v,R;

    //printf("\nTama?o del vector: ");
    scanf("%d",&n);
    v=(int *) malloc(sizeof(int)*n);
    for(x=0;x<n;x++){
       scanf("%d",&v[x]);
    } 
    R=inversiones(v,0,n);
    printf("%d\n",R);
    return 0;
}
