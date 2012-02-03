#include<iostream>

using namespace std;

int cont=0;
int sumasi44(int uno,int q,int p,int nn);
int A[300][300];

int main(){
int n,p,q;
 for(int ar=0; ar<300;ar++)
    for(int ab=0; ab<300;ab++)
       A[ar][ab]=-1;
    cin>>n;
    cin>>p;
    cin>>q;
    sumasi44(n,q,p,0);
    cout<<cont<<endl;
}

int sumasi44(int uno,int q,int p,int nn){
int i,j;
//if (A[uno][q]>-1)
   if(uno>0){
     if(q>=p){
      if(uno<=q)
         j=uno;
      else
         j=q;
	  for(i=1;i<=j;i++){
      A[uno][q]=sumasi44(uno-i,i,p,q);
	  }
     }
   }
   else
     if(q>=p && q!=nn)
       cont++;
}
