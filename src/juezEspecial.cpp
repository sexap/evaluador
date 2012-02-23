#include "juezEspecial.h"

int juezEspecial(const string& casoDePrueba, const string& judgeExe)
{
    ofstream cout("logJE.txt", fstream::app);

    int calif;
    pid_t pID;
    int status, programa;

	pID = fork();
	if (pID < 0) {
		cout << "No se pudo hacer el fork para correr el juez especial" << endl;
	}
	else if (pID == 0) {
		//Redirijo Caso de Prueba --> Entrada juez especial
		if (freopen(casoDePrueba.c_str(), "r", stdin) == NULL) {
			cout << "Falló freopen 1 JE" << endl;
		}

		//Redirijo Salida JE --> archivo de veredictoJE
		if (freopen("veredictoJE", "w", stdout) == NULL) {
			cout << "Falló freopen 2 JE" << endl;
		}

		//Ejecuto el JE
		programa = execl(judgeExe.c_str(), judgeExe.c_str(), (char *)NULL);
		if (programa < 0) {
            //Si no se logra ejecutar correctamente el programa, se guarda un RE (runtime error)
            cout << "Error de ejecución del JE" << endl;
            return 0;
        }
	} // Termina el hijo

	waitpid(pID, &status, 0);
	cout << "Terminó la evaluación especial del caso " << casoDePrueba << endl;

	ifstream veredictoJE("veredictoJE");

	if (veredictoJE.fail()) {
		cout << "No pudo abrir veredictoJE" << endl;
	}

	if(veredictoJE >> calif)
	{
		cout << "Caso " << getFileName(casoDePrueba) << " sacó " << calif << endl;
	}
	else
	{
		cout << "El JE no dio veredicto..." << endl;
	}

	cout.close();
    return calif;
}
