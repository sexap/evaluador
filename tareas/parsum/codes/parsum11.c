#include <stdio.h>
int main(){
char c;
int i=0;
while((c=getchar())!='\n')
	if(c=='(')i++;
printf("%d\n",i);
return 0;
}