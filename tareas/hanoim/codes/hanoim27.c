
#include <stdio.h>

void Thanoi(int, int, int, int);

int main(){
	int n = 0;
	scanf("%i", &n);
	Thanoi(n, 1, 2, 3);
	printf("0 0\n");
	return 0;
}

void Thanoi(int n, int A, int B, int C){

	if(n > 0){
		Thanoi(n-1, A, B, C);
		printf("%i %i\n", A, B);
		//B = A;
		Thanoi(n-1, C, B, A);
		printf("%i %i\n", B, C);
		//C = B;
		Thanoi(n-1, A, B, C);
	}
}
