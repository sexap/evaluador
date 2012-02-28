#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

bool valido(int ro, int co, int rd, int cd)
{
	int dr, dc;
	dr = abs(ro - rd);
	dc = abs(co - cd);
	return ( (dr == 2 && dc == 1) || (dr == 1 && dc == 2) );
}

int main(int argc, char* argv[])
{
	//Entrada:
		//Tamaño del tablero (n), posición inicial r, c

	//Salida:
		//Tablero con las posiciones visitadas

	//Chequeo:
		//Durante la lectura:
			//No repita números
			//No falten numeros de 1..pm
			//Que sólo sean n*n caracteres
			//Que sólo sean caracteres válidos

		//Durante el procedimiento:
			//Debe tener un recorrido que tome el número mínimo de pasos.
			//Cada movimiento debe ser legal.
			//El último movimiento lo debe dejar atorado.

	ifstream casoent(argv[1]);
	ifstream casodat(argv[2]);
	int calif = 100;

	if(!casoent.good())
        cerr << "No se puede leer el archivo de caso de prueba." << endl;
    else
        cerr << "Abrí el archivo de caso de prueba: " << argv[1] << endl;

	if(!casodat.good())
        cerr << "No se puede leer el archivo de caso de prueba." << endl;
    else
        cerr << "Abrí el archivo de caso de prueba: " << argv[2] << endl;

	/*************************/
	int n, ri, ci, paso, minPasos;
	int leidos[401][3] = {}; //0 (leido o no), 1 (ren), 2 (col)
	int tablero[20][20];
	int vecinos[8][2] = {{+2,+1}, {+1,+2}, {-1,+2}, {-2,+1}, {-2,-1}, {-1,-2}, {+1, -2}, {+2, -1}};

	//Leo los datos del caso
	casoent >> n >> ri >> ci;
	casodat >> minPasos;

	//Leo el tablero que dejó el alumno
	for(int ren = 0; ren < n; ren++)
	{
		for(int col = 0; col < n; col++)
		{
			cin >> paso;
			tablero[ren][col] = paso;

			//Checo que esté en rango 0..minPasos
			if(paso < 0  )
			{
				calif = 0;
				cerr << "Dio un paso negativo " << ren << ", " << col << endl;
				break;
			}
			if(paso > minPasos)
			{
				calif = 0;
				cerr << "Dio un paso de más " << paso << endl;
				break;
			}

			//Checho que no lo haya leido
			if(leidos[paso][0] == 1)
			{
				calif = 0;
				cerr << "Repitió paso " << paso << endl;
				break;
			}
			if(paso > 0)
			{
				leidos[paso][0] = 1;
				leidos[paso][1] = ren;
				leidos[paso][2] = col;
			}
		}
		if(calif == 0) break;
	}
	if(cin.fail())
	{
		cerr << "Algo no era número." << endl;
		calif = 0;
	}

	//Checo que estén todos los pasos de 1 a mp
	for(int I = 1; I <= minPasos; I++)
	{
		if(leidos[I][0] == 0)
		{
			cerr << "Faltó algún paso" << endl;
			calif = 0;
			break;
		}
	}

	//Checo que el primer paso es el ri ci
	if(leidos[1][1] != ri-1 || leidos[1][2] != ci-1)
	{
		cerr << "No empieza donde debe" << endl;
		calif = 0;
	}

	//Checo que cada paso sea válido
	for(int I = 1; I < minPasos; I++)
	{
		if(!valido(leidos[I][1], leidos[I][2], leidos[I+1][1], leidos[I+1][2]))
		{
			cerr << "Caballo dio mal paso" << endl;
			calif = 0;
			break;
		}
	}

	//Checo que el caballo quede encerrado en la última posición
	//Checo que ninguno de los 8 vecinos sean 0
	int posx = leidos[minPasos][1];
	int posy = leidos[minPasos][2];

	for (int i = 0; i < 8; ++i) {
        posx += vecinos[i][0];
        posy += vecinos[i][1];
        if (posx >= 0 && posx < n && posy >= 0 && posy < n) {
            if (tablero[posx][posy] == 0) {
            	cerr << "No quedó encerrado" << endl;
				calif = 0;
				break;
            }
        }
        posx -= vecinos[i][0];
        posy -= vecinos[i][1];
    }
	/*************************/

	cout << calif << endl;

	casoent.close();
	casodat.close();
	return 0;
}
