#include "juezInteractivo.h"

pid_t juezInteractivoStart(const string& caseFile, const string& judgeExe, int alumno_in, int alumno_out) {
	pid_t pID;
	ParamHolder jParams;

	pID = fork();
	if (pID < 0) {
		cerr << "Falló fork() para Juez Interactivo" << endl;
        perror("Error");
        return 1;
	}
	else if (pID == 0) {
		//Redirijo salida -> alumno_in
		dup2(alumno_in, STDOUT_FILENO);
		// Redirige entrada <- alumno_out
		dup2(alumno_out, STDIN_FILENO);

		jParams.add(judgeExe);
		jParams.add(caseFile);
		jParams.add("veredicto");

		//Ejecuto el JI
		clog << "  Ejecutando juez interactivo con comando '" << jParams << "'" << endl;
		if (execvp(jParams.exe(), jParams.params()) < 0) {
            cerr << "Falló exec() para la ejecución del Juez Interactivo" << endl;
			perror("Error");
        }
        return 1;
	} // Termina el hijo

	return pID; // Termina la función
}

bool juezInteractivoEnd(int status) {
    bool goodRun;

	// Verificar que haya finalizado correctamente
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
		clog << "  Ejecución de Juez Interactivo correcta..." << endl;
		goodRun = true;
	}
	else {
		clog << "  Falló la ejecución del Juez Interactivo." << endl;
		goodRun = false;

		if (WIFEXITED(status)) {
			clog << "  Terminó misteriosamente con valor de retorno " << WEXITSTATUS(status) << endl;
		}
		else if (WIFSIGNALED(status)) {
			clog << "  Matado misteriosamente por la señal " << WTERMSIG(status) << endl;
		}
	}
	return goodRun;
}

int juezInteractivoResult() {
	ifstream califFile("veredicto");
	int calif;

	clog << "  Analizando veredicto..." << endl;
	if (califFile.fail()) {
		clog << "  No pudo abrir veredicto de JI" << endl;
	}

	if(califFile >> calif)
	{
		clog << "  Caso x sacó " << calif << endl;
	}
	else
	{
		clog << "  El JI no dio veredicto..." << endl;
	}

	remove("veredicto");
    return calif;
}
