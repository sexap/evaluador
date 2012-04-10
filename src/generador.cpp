#include "generador.h"

bool generator(const string& generator, const string& tempCase)
{
    pid_t pID;
    int status;
    bool goodRun;
	ParamHolder jeParams;

	pID = fork();
	if (pID < 0) {
		cerr << "Falló fork() para Generador" << endl;
        perror("Error");
        return 1;
	}
	else if (pID == 0) {
		//Redirijo Caso de Prueba
		freopen("/dev/null", "r", stdin);

		//Redirijo Salida Generador
		freopen(tempCase.c_str(), "w", stdout);

		//Redirijo Salida stderr del Generador
		freopen("errGEN", "a", stderr);

		jeParams.add(generator);

		//Ejecuto el Generador
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
		clog << "  Ejecución del Generador correcta..." << endl;
		goodRun = true;
	}
	else {
		clog << "  Falló la ejecución del Generador." << endl;
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
