#include <stdio.h>

void mezcla(int i, int j);
void pega(int i, int m, int j);
int   m, a[1000], y[1000], n, i, t=1;;

int main()
{	
	
	scanf("%d", &n);
	for (i=1; i<=n; i++)
	scanf("%d", &a[i]);
	mezcla(t, n);
	return 0;
}


void mezcla(int i, int j)	
{
	//printf ("valor de i=%d j=%d\n", i, j);
	if (i<j)
	{
		m=(i+j)/2;
		mezcla(i, m);
		mezcla(m+1, j);	
		pega(i, m, j);	
	}
}

void pega(int i, int m, int j)
{
	int k=i, l=m+1, h=i, x;
	//printf ("EL VALOR DE K=%d osea de i L=%d osea de m+1 H=%d osea de j\n", k, l, h);
	while(k<=m && l<=j) 
	{
		//printf ("a[k] %d\n", a[k]);
		//printf ("a[l] %d\n", a[l]);	
		if(a[k]<=a[l])
		{
			
			y[h]=a[k];	
			k=k+1;
			
			//printf ("if1 %d\n", y[h-1]);
		}
		
		else 
		{
			y[h]=a[l];
			l=l+1;
			h=h+1;
			//printf ("else1 %d\n", y[h-1]);

		}
		
		if(k>m)
		{
			while(l<=j)
			{
				y[h]=a[l];
				l=l+1;
				h=h+1;
			}
			//printf ("if2 %d\n", y[h-1]);
		}
		
		else 
		{
			while(k<=m)
			{
				y[h]=a[k];
				k=k+1;
				h=h+1;
			}
			//printf ("else2 %d\n", y[h-1]);
		}
		
	}
	for (x=1; x<=n; x++)
	{
		//printf ("%d\t", y[x]);	
		//printf ("\n");
	}
}

