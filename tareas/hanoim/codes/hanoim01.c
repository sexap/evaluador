#include <stdio.h>

void hanoi(int n,int A, int B, int C){
	if(n==1){
		printf("%d %d\n",A,B);
		printf("%d %d\n",B,C);
    }
	else{
		hanoi(n-1,A,B,C);
		printf("%d %d\n",A,B);
		hanoi(n-1,C,B,A);
		printf("%d %d\n",B,C);
		hanoi(n-1,A,B,C);
	}
}

int main(){
        int A=1,B=2,C=3;
        int n;
        scanf("%d",&n);
        hanoi(n,A,B,C);
        printf("0 0\n");
return 0;
}

