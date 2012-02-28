#include <iostream>
#include <fstream>
#include <stack>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
	//Checa que cada paso sea movimiento legal
		//No haya discos grandes sobre chicos.
		//Sólo sean movimientos que pasen por el centro

    int desde, hacia, numDiscos;
	int correcto = 100;
	stack<int> torres[4];
	ifstream caso(argv[1]);

	if(!caso.good())
        cerr << "No se puede leer el archivo de caso de prueba." << endl;
    else
        cerr << "Abrí el archivo de caso de prueba: " << argv[1] << endl;

	caso >> numDiscos;

	//Coloco los discos en la primera torre
	for(int I = 0; I < numDiscos; I++)
	{
		torres[1].push(numDiscos-I);
	}

	//Leo cada uno de los movimientos del alumno y los simulo
	while(cin >> desde >> hacia)
	{
		if(desde == 0 && hacia == 0)
			break;

		//Checa que sólo sean 1, 2, o 3
		if(desde != 1 && desde != 2 && desde != 3)
		{
			cerr << "Torre invalida: " << desde << endl;
			correcto = 0;
			break;
		}
		if(hacia != 1 && hacia != 2 && hacia != 3)
		{
			cerr << "Torre invalida: " << hacia << endl;
			correcto = 0;
			break;
		}

		//Checa que todos pasen por el centro
		if((desde == 1 && hacia == 3) || (desde == 3 && hacia == 1))
		{
			correcto = 0;
			cerr << "Movimiento 1-3 o 3-1" << endl;
			break;
		}

		//Checa que no intente quitar de torre vacía
		if(torres[desde].empty())
		{
			correcto = 0;
			cerr << "Intenta quitar de torre vacía" << endl;
			break;
		}

		int cualQuito = torres[desde].top();
		torres[desde].pop();
		//Checa que no ponga grande sobre chico
		if(!torres[hacia].empty() && torres[hacia].top() <= cualQuito)
		{
			correcto = 0;
			cerr << "Intento poner grande sobre chico." << endl;
			break;
		}
		else
		{
			torres[hacia].push(cualQuito);
		}
	}

	//Checa que el estado final sea correcto
	if(!torres[1].empty() || !torres[2].empty())
	{
		correcto = 0;
		cerr << "El estado final no es correcto" << endl;
	}

	cout << correcto << endl;

	caso.close();
	return 0;
}
