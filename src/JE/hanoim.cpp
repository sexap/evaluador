#include <iostream>

using namespace std;

int main()
{
	//Checa que cada paso sea movimiento legal
		//No haya discos grandes sobre chicos.
		//Sólo sean movimientos que pasen por el centro

	//Checa que el resultado sea correcto

/*	ifstream respCorrecta(archSalidaEsperada.c_str());
    ifstream respAlumno(archAlum.c_str());
	string archAlum = "salida_exec_alumno";

	if(!respAlumno.good())
        cout << "No se puede leer el archivo de resultado del alumno." << endl;
    else
        cout << "Abrí el archivo del alumno." << endl;
    if(!respCorrecta.good()){
        cout << "No se puede abrir la salida esperada." << endl;
        return false;
    }
    else{
        cout << "Abrí la respuesta correcta." << endl;
    }



	respCorrecta.close();
    respAlumno.close();
*/
	bool correcto = true;
	cout << correcto;
	return 0;
}
