#include<iostream>
#include<string>
#include<sstream>

using namespace std;

	bool V[1001][1001][1001];
 	unsigned short R[1001][1001][1001];

int N,K,P,padre;

int part(int n, int m, int padre);

main(){


/*	for(int i=0;i<1000;i++)
		for(int j=0;j<1001;j++)
			for(int k=0;k<1001;k++){
				V[i][j][k]=false;
				R[i][j][k]=0;
			} */
	

	int m, Q;
	char linea[15];
	string cadena;
	
	cin.getline( linea, 12, '\n' );
	cadena=linea;
	istringstream icadena(cadena);
	
	icadena >> N;
	icadena >> P;
	icadena >> Q;

	cout << part(N,Q,padre) << endl;
}

int part(int n, int m, int padre){
//	cout<<n<<' '<<m<<' '<<padre<<endl;
if(V[n][m][padre]==false){
	int min;
	if(n>0){
		if(n>=P && m>=P){
			int F=0;
			for(int i=P;i<=m;i++){
				if(i!=padre){
					if(i<=n-i)
						min=i;
					else
						min=n-i;
					F=F+part(n-i,min,i);
				}
			}
			V[n][m][padre]==true;
			R[n][m][padre]==F;
			return F;
		}
		else{
			V[n][m][padre]==true;
			R[n][m][padre]==0;
			return 0;
		}
	}
	else	
		V[n][m][padre]==true;
		R[n][m][padre]==1;
		return 1;
}
else
	return R[n][m][padre];
}

