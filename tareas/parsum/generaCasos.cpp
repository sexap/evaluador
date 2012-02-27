#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;

int main()
{
	int cuantos = 1024;
	int suma = 0;
	ofstream cout("salEsp10.dat");
	ofstream paren("caso10.ent");
	for(int I = 0; I < cuantos; I++)
	{
		int cual = (rand()%10000+1);
		suma += cual;
		for(int I = 0; I < cual; I++)
			paren << "(";
		for(int I = 0; I < cual; I++)
			paren << ")";
	}
	cout << suma;
	return 0;
}
