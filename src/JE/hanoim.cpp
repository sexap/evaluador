#include <iostream>
#include <fstream>
#include <stack>
#include <string>

using namespace std;

int main()
{
	//Checa que cada paso sea movimiento legal
		//No haya discos grandes sobre chicos.
		//Sólo sean movimientos que pasen por el centro

	//Checa que el resultado sea correcto

	string archAlum = "../salida_exec_alumno";
    ifstream respAlumno(archAlum.c_str());
    int desde, hacia, numDiscos;
	int correcto = 100;
	stack<int> torres[4];

	if(!respAlumno.good())
        cout << "No se puede leer el archivo de resultado del alumno." << endl;
    else
        cout << "Abrí el archivo del alumno." << endl;

	cin >> numDiscos;

	//Coloco los discos en la primera torre
	for(int I = 0; I < numDiscos; I++)
	{
		torres[1].push(numDiscos-I);
	}

	//Leo cada uno de los movimientos del alumno y los simulo
	while(respAlumno >> desde >> hacia)
	{
		if(desde == 0 && hacia == 0)
			break;

		int cualQuito = torres[desde].top();
		torres[desde].pop();
		if(torres[hacia].top() <= cualQuito)
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

	if(!torres[1].empty() || !torres[2].empty())
	{
		correcto = 0;
		cerr << "El estado final no es correcto" << endl;
	}

	cout << correcto;

	respAlumno.close();
	return 0;
}
