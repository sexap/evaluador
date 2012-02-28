#include <stdio.h>
#include <stdlib.h>
unsigned int A[21][21], B[21][21], Apasos=400;
unsigned int Cflojo(int Ncaballo,int N,int R,int C,int pasos)
{
	if (Ncaballo<=(N*N) && pasos<Apasos)
	{
		if (((1<=C)&&(C<=N))&&((1<=R)&&(R<=N)))
		{
			A[R][C]=Ncaballo;
			unsigned int k=0;
			for(k=0;k<=8;k++)
			{
				switch (k)
				{
				case 0:
					if(A[R-2][C-1]==0)
					{
				
					Cflojo(Ncaballo+1,N,R-2,C-1,pasos+1);
					
					}
					break;
				case 1:
					if(A[R-1][C-2]==0)
					{
				
					Cflojo(Ncaballo+1,N,R-1,C-2,pasos+1);
					
					}
					break;
				case 2:
					if(A[R+1][C-2]==0)
					{
			
					Cflojo(Ncaballo+1,N,R+1,C-2,pasos+1);
					
					}
					break;		
				case 3:
					if(A[R+2][C-1]==0)
					{
					
					Cflojo(Ncaballo+1,N,R+2,C-1,pasos+1);
					
					}
					break;
				case 4:
					if(A[R+2][C+1]==0)
					{
					
					Cflojo(Ncaballo+1,N,R+2,C+1,pasos+1);
					
					}
					break;
				case 5:
					if(A[R+1][C+2]==0)
					{
					
					Cflojo(Ncaballo+1,N,R+1,C+2,pasos+1);
					
					}
				break;
				case 6:
					if(A[R-1][C+2]==0)
					{
					
					Cflojo(Ncaballo+1,N,R-1,C+2,pasos+1);
					
					}
					break;
				case 7:
					if(A[R-2][C+1]==0)
					{
					
					Cflojo(Ncaballo+1,N,R-2,C+1,pasos+1);
					
					}
					break;
				default:
				break;
				}

			}
			if (A[R-2][C-1]!=0 && A[R-1][C-2]!=0 && A[R+1][C-2]!=0 && A[R+2][C-1]!=0 && A[R+2][C+1]!=0 && A[R+1][C+2]!=0 && A[R-1][C+2]!=0 && A[R-2][C+1]!=0 && pasos<Apasos)
					{
						int i=0,j=0;
						Apasos=pasos;
						for(i=0;i<=20;i++)
						{
							for(j=0;j<=20;j++)B[i][j]=A[i][j];
						}/*
						unsigned int w,z;
						for(w=1;w<=N;w++)
						{
							for(z=1;z<=N;z++)
							{
								printf("%u ",A[w][z]);
							}
						printf("\n");
						}
						//exit(0);
						system("pause");*/
					}
			A[R][C]=0;
		}
		}
		else
		{
			
		}
	
	return 0;
}

int main()
{
	unsigned int i=0,j=0, Ncaballo=1,pasos=1;
	unsigned int r=0,c=0,n=0;
	
	
	for(i=0;i<=20;i++)
		{
			for(j=0;j<=20;j++)B[i][j]=0;
		}
	for(i=0;i<=0;i++)
		{
			for(j=0;j<=20;j++)B[i][j]=1;
		}
	for(j=0;j<=0;j++)
		{
			for(i=0;i<=20;i++)B[i][j]=1;
		}
	
	scanf("%u %u %u",&n,&r,&c);
	for(i=0;i<=20;i++)
		{
			for(j=0;j<=20;j++)A[i][j]=1;
		}
	for(i=0;i<=n;i++)
		{
			for(j=0;j<=n;j++)A[i][j]=0;
		}
	for(i=0;i<=0;i++)
		{
			for(j=0;j<=20;j++)A[i][j]=1;
		}
	for(j=0;j<=0;j++)
		{
			for(i=0;i<=20;i++)A[i][j]=1;
		}
	Cflojo(Ncaballo,n,r,c,pasos);
	
	unsigned int w,z;
	if(n==1)
	{
	printf("1\n");
	}
	else if(n==2)
	{
		if(r==1 && c==1){printf("1 0\n0 0\n");}
		if(r==1 && c==2){printf("0 1\n0 0\n");}
		if(r==2 && c==1){printf("0 0\n1 0\n");}
		if(r==2 && c==2){printf("0 0\n0 1\n");}
	}
	else
	{
		for(w=1;w<=n;w++)
		{
			for(z=1;z<=n;z++)
			{
				printf("%u ",B[w][z]);
			}
			printf("\n");
		}
	}
	return 0;	
}
