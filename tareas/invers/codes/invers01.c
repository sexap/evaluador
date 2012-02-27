/*Celene Dorali Alfaro Quintero
208301193*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int b[50]={},in=0;

void Mezcla(int a[],int i,int j){
int m,cl;
if(i<j){
  if(a[i]>a[j])   
    in++;   
  m=(i+j)/2;
  Mezcla(a,i,m);
  Mezcla(a,m+1,j);
  Combina(a,i,m,j);
}
}

Combina(int a[], int i, int m, int j){
int k,q,aux=i,l;
k=m+1;
l=i;
 while((i<=m)&&(k<=j)){
    if(a[i]<a[k]){
       b[l]=a[i];
       i++;  
    }
    else{
       b[l]=a[k];
       k++;
       in++;
    }
    l++;
 }
 while(i<=m)
     b[l++]=a[i++];
 
 while(k<=j)
     b[l++]=a[k++];
 for(i=aux;i<l;i++)
     a[i]=b[i];
} 

int main(){
int n=0,c,*a;
char lin[100]="",lin2[100]="",*tok;
fgets(lin,100,stdin);
sscanf(lin,"%s",&lin2);
n=atoi(lin2);
fgets(lin,100,stdin);
a=(int*)malloc(sizeof(int)*n);
tok=strtok(lin," ");
c=0;
while(tok!=NULL){
   a[c]=atoi(tok);
   c++;
   tok=strtok(NULL,"\n ");
}
Mezcla(a,0,n-1);
printf("%d\n",in);
return 0;
}
