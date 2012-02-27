#include<stdio.h>
unsigned long long int sylvester(int n){
  unsigned long long int temp;
  if (n==0)
    return 2;
  else {
    temp = sylvester(n-1);
    return temp*(temp-1)+1;
  }
}
int main(void){
  int n,m;
  scanf("%d %d",&n,&m);
  printf("%d",sylvester(n)%m);
  return 0;
}
