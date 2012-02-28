#include "juezEspecial.h"

int juezEspecial(const string& casoDePrueba, const string& judgeExe)
{
    int calif;
    pid_t pID;
    int status;
    bool goodRun;
	ParamHolder jeParams;

	pID = fork();
	if (pID < 0) {
		cerr << "Falló fork() para Juez Especial" << endl;
        perror("Error");
        return 1;
	}
	else if (pID == 0) {
		//Redirijo Caso de Prueba --> Entrada juez especial
		freopen("salida_exec_alumno", "r", stdin);
		//Redirijo Salida JE --> archivo de veredictoJE
		freopen("veredictoJE", "w", stdout);

		jeParams.add(judgeExe);
		jeParams.add(casoDePrueba + "." + CASE_EXTENSION);
		if(isFile(casoDePrueba + "." + OUTPUT_EXTENSION)) jeParams.add(casoDePrueba + "." + OUTPUT_EXTENSION);

		//Ejecuto el JE
		if (execvp(jeParams.exe(), jeParams.params()) < 0) {
            cerr << "Falló exec() para la ejecución" << endl;
			perror("Error");
        }
        return 1;
	} // Termina el hijo

	// Esperar hijo
	waitpid(pID, &status, 0);
	// Verificar que haya finalizado correctamente
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
		clog << "  Ejecución de Juez Especial correcta..." << endl;
		goodRun = true;
	}
	else {
		clog << "  Falló la ejecución del Juez Especial." << endl;
		goodRun = false;

		if (WIFEXITED(status)) {
			clog << "  Terminó misteriosamente con valor de retorno " << WEXITSTATUS(status) << endl;
		}
		else if (WIFSIGNALED(status)) {
			clog << "  Matado misteriosamente por la señal " << WTERMSIG(status) << endl;
		}
	}
	// Si no acabó bien devolver calif 0
	if (!goodRun) return 0;

	clog << "  Analizando veredicto..." << endl;

	ifstream veredictoJE("veredictoJE");
	if (veredictoJE.fail()) {
		clog << "  No pudo abrir veredictoJE" << endl;
	}

	if(veredictoJE >> calif)
	{
		clog << "  Caso " << getFileName(casoDePrueba) << " sacó " << calif << endl;
	}
	else
	{
		clog << "  El JE no dio veredicto..." << endl;
	}

	remove("veredictoJE");
    return calif;
}
