#include<stdio.h>

#include<math.h>


int sylvester(int);

int main()

{

  int d,c;


  scanf("%d %d",&d,&c);

  printf("%d",sylvester(d)%c);




 return 0;



}



int sylvester(int N)
{
  int a;

  if(N==0)
     return 2;
  else
   {
     a=sylvester(N-1);    
     a=(a*(a-1))+1;
     
     return a; 
   }


}