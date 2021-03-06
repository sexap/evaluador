// Librerias estándar
#include <iostream>
#include <fstream>
#include <list>
#include <stack>
#include <string>
#include <sstream>
using namespace std;

// Librerias C
#include <cstdlib>
#include <cstdio>

// Librerias linux
#include <unistd.h> //execl
#include <sys/time.h> //setrlimit
#include <sys/resource.h> //setrlimit
#include <sys/wait.h>

// Librerias propias
#include "build_params.h"
#include "utils/config.h"
#include "utils/reporte.h"
#include "utils/validation.h"
#include "utils/process_control.h"
#include "juezNormal.h"
#include "juezEspecial.h"
#include "juezInteractivo.h"
#include "generador.h"

using namespace seap_implement;

streambuf* original_buf_clog = clog.rdbuf();
void endProgram(void) {
	// Regresa clog a su estado esperado
	clog.rdbuf(original_buf_clog);
}

int main(int argc, char* argv[])
{
	/*
	cerr irá a pantalla: Mensajes de error interesantes para el usuario
		errores fatales van directo a cerr.
			Ejemplo: cerr << "Error" << endl;
		errores NO fatales van a cerr sólo si verbose == true
			Ejemplo: if (verbose) cerr << "Advertencia" << endl;
	clog irá a archivo: Mensajes de estado interesantes para nosotros
	*/
	ofstream logfile("log.txt");
	clog.rdbuf(logfile.rdbuf());
	atexit(endProgram);

	/*
	Variables que se pueden usar tras la validación
		string judgeType, judgeExe, action, problem, outputFile;
		int maxSourceSize, maxCompTime, maxCompMem, maxOutSize, maxRunTime, maxRunMem;
		bool verbose, showProgress, strictEval, compareWhite;
		list<string> testCases, sourceFiles;
	*/
    #include "main_validation.cpp"

	/*
	Variables que se pueden usar tras la evaluación
		--
	*/
	#include "main_evaluation.cpp"

    return 0;
}
