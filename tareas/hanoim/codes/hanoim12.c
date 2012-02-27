#include <stdio.h>

#include <stdlib.h>



void hanoi(int n,int i, int x, int f);





int main(){


    
	int i=1,x=2,f=3,n;

	

	//printf("\nN??mero de discos: ");


	scanf("%d",&n);


	//printf("\n\nLos movimientos a realizar son: \n");


	
if(n>0)
hanoi(n,i,x,f);


	printf("0 0");


//
	//system("pause");


	return 0;
}



void hanoi(int n,int i,int x,int f){



	if(n==1){

		printf("%d %d\n",i,x);

		printf("%d %d\n",x,f);

	}

	else{

		hanoi(n-1,i,x,f);

		printf("%d %d\n",i,x);

		//printf("%d %d\n",f,i);

		hanoi(n-1,f,x,i);

	        //printf("%d %d\n",i,x);

		printf("%d %d\n",x,f);
		hanoi(n-1,i,x,f);

        }

}


