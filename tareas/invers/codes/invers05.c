#include<stdio.h>

void mezcla(unsigned long array[],int i, int j);
void combina(unsigned long array[],int i, int m, int j);
int cont=0;

int main()
{
    int a;
    unsigned long N;
    scanf("%ld",&N);
    unsigned long A[N];
    for(a=0;a<N;a++)
    {
        scanf("%ld",&A[a]);
    }
    //system("clear");
    mezcla(A,0,N-1);
    printf("%d",cont);
    return 0;
}

void mezcla(unsigned long array[],int i, int j)
{
    int m;
    if (i<j)
    {
        m=(i+j)/2;
        mezcla (array,i,m);
        mezcla (array,m+1,j);
        combina (array,i,m,j);
    }
    else
    {
        return;
    }
}


void combina(unsigned long array[],int i, int m, int j)
{
    int cont_interno=0;
    int inicio;
    int k;
    int l=0;
    unsigned long c[j-i+1];
    k = m+1;
    inicio=i;
    while (i<=m && k<=j)
    {
        if (array[i] < array[k])
        {
            c[l] = array[i];
            i++;
        }
        else
        {
            c[l] = array[k];
            k++;
            cont_interno=cont_interno+(m-i+1);
        }
        l++;
    }
    while (i<=m)
    {
        c[l] = array[i];
        l++;
        i++;
    }
    while (k<=j)
    {
        c[l] = array[k];
        l++;
        k++;
    }
    for (l=0; l<j-inicio+1; l++)
    {
        array[inicio+l] = c[l];
    }
    cont=cont+cont_interno;
}
