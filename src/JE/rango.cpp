#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
	//La entrada estandar es la salida esperada.
	ifstream respAlum("../salida_exec_alum");

	int min, max, numAlum, accepted = 1;	
	string cadenaAlum;

	cin >> min >> max;
	/*while(respAlum >> cadenaAlum)
	{
		cout << cadenaAlum;
	}
	respAlum.close();
	respAlum.open("../salida_exec_alum");*/
	respAlum >> numAlum;
	if(respAlum >> cadenaAlum)
	{	
		accepted = 0;
	}
	else
	{
		if(numAlum < min || numAlum > max)
			accepted = 0;
	}
	cout << accepted;
	return 0;
}
