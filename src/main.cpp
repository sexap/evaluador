#include <iostream>
#include <list>
#include <stack>
#include <string>
using namespace std;

#include "config.h"
#include "validation.h"
using namespace seap_implement;

#include <fstream>
#include <sys/types.h>
#include <unistd.h> //execl
#include <cstdlib> //execl
#include <sys/time.h> //setrlimit
#include <sys/resource.h> //setrlimit
#include <sys/wait.h>
#include <cstdio>

int main(int argc, char* argv[]) {
	Config confArg, confFile;
	string lang, judgeType, judgeExe, action, problem;
	int maxSourceSize, maxCompTime, maxCompMem, maxOutSize, maxRunTime, maxRunMem;
	bool verbose, strictEval, compareWhite, hasError;
	list<string> testCases, tmpList, sourceFiles;
	stack<list<string>::iterator> toErase;

	// Registra variables
	confArg.registerArgVar("action", Config::T_STRING, 1);
	confArg.registerArgVar("problem", Config::T_STRING, 2);

	confArg.registerArgVar("f", Config::T_LIST, true);
	confArg.registerArgVar("l", Config::T_STRING, false);
	confArg.registerArgVar("s", Config::T_INT, false);
	confArg.registerArgVar("T", Config::T_INT, false);
	confArg.registerArgVar("M", Config::T_INT, false);
	confArg.registerArgVar("S", Config::T_INT, false);
	confArg.registerArgVar("c", Config::T_LIST, false);
	confArg.registerArgVar("v", Config::T_BOOL, false);

	confFile.registerFileVar("max_time", Config::T_INT, true);
	confFile.registerFileVar("max_mem", Config::T_INT, false);
	confFile.registerFileVar("judge_type", Config::T_STRING, true);
	confFile.registerFileVar("judge_exe", Config::T_STRING, false);
	confFile.registerFileVar("strict_eval", Config::T_BOOL, false);
	confFile.registerFileVar("compare_white", Config::T_BOOL, false);

	// Valores por default
	confArg.setValue("l", "auto"); // Lenguaje auto
	confArg.setValue("s", 24); // 24kB de código
	confArg.setValue("T", 5000); // 5s para compilar
	confArg.setValue("M", 4096); // 4MB para compilar (revisar)
	confArg.setValue("S", 8); // 8KB de salida
	confArg.setValue("v", false); // Es callado

	confFile.setValue("max_mem", 32768); // 32MB para ser ejecutado (revisar)
	confFile.setValue("judge_exe", "judge"); // Ejecutable del juez
	confFile.setValue("strict_eval", false); // No es estricto
	confFile.setValue("compare_white", false); // Ignora espacios extra

	// Analizar y valida argumentos
	confArg.parseArgs(argc, argv);
	if (!confArg.validate()) {
		cout << "Parametros incorrectos. Consulte el manual" << endl;
		return 1;
	}

	confArg.getValue("action", action);
	confArg.getValue("problem", problem);

	confArg.getValue("f", sourceFiles);
	confArg.getValue("l", lang);
	confArg.getValue("s", maxSourceSize);
	confArg.getValue("T", maxCompTime);
	confArg.getValue("M", maxCompMem);
	confArg.getValue("S", maxOutSize);
	confArg.getValue("v", verbose);
	confArg.getValue("c", testCases);

	hasError = false;
	if (!isValidAction(action)) {
		cout << action << " no es una acción válida" << endl;
		hasError = true;
	}
	if (!isDir(problem)) {
		cout << "No existe el problema " << problem << endl;
		hasError = true;
	}
	if (!isFile(problem + "/eval.conf")) {
		cout << "No se encuentra " << problem << "/eval.conf" << endl;
		hasError = true;
	}
	if (!isBetween(maxSourceSize, 1, 512)) {
		cout << "el parametro -s debe estar entre 1kB y 512kB" << endl;
		hasError = true;
	}
	if (!isBetween(maxCompTime, 1, 90000)) {
		cout << "el parametro -T debe estar entre 1ms y 90s" << endl;
		hasError = true;
	}
	if (!isBetween(maxCompMem, 1, 262144)) { // (revisar)
		cout << "el parametro -M debe estar entre 1kB y 256MB" << endl;
		hasError = true;
	}
	if (!isBetween(maxOutSize, 1, 256)) {
		cout << "el parametro -S debe estar entre 1kB y 256kB" << endl;
		hasError = true;
	}
	if (hasError) return 1;

	// Analizar y valida archivo de configureción
	confFile.parseFile(problem + "/eval.conf");
	if (!confFile.validate()) {
		cout << "Archivo erroneo. Consulta el manual" << endl;
		return 1;
	}

	confFile.getValue("max_time", maxRunTime);
	confFile.getValue("max_mem", maxRunMem);
	confFile.getValue("judge_type", judgeType);
	confFile.getValue("judge_exe", judgeExe);
	confFile.getValue("strict_eval", strictEval);
	confFile.getValue("compare_white", compareWhite);

	hasError = false;
	if (!isBetween(maxRunTime, 1, 60000)) {
		cout << "max_time debe estar entre 1ms y 60s" << endl;
		hasError = true;
	}
	if (!isBetween(maxRunMem, 1, 262144)) {
		cout << "max_mem debe estar entre 1kB y 256MB" << endl;
		hasError = true;
	}
	if (!isValidJudgeType(judgeType)) {
		cout << judgeType << " no es un tipo de juez válido" << endl;
		hasError = true;
	}
	else if (judgeNeedsExe(judgeType)) {
		if (!isFile(judgeExe)) {
			cout << "No se encuentra el ejecutable " << judgeExe << endl;
			hasError = true;
		}
	}
	if (hasError) return 1;

	// Genera lista de casos de prueba
	hasError = false;
	if (testCases.size() == 0) {
		testCases = Config::getDir(problem, "case");
		// Quita extensión
		for (list<string>::iterator it = testCases.begin(); it != testCases.end(); it++) {
			*it = it->substr(0, it->rfind('.'));
		}
	}
	else {
		for (list<string>::iterator it = testCases.begin(); it != testCases.end(); it++) {
			if (!isFile(*it)) {
				cout << "No existe el caso " << *it << endl;
				hasError = true;
			}
			else if (hasExtension(*it, "set")) {
				tmpList = Config::getSet(problem + "/" + *it);
				testCases.insert(testCases.end(), tmpList.begin(), tmpList.end());
				toErase.push(it);
			}
			else if (judgeNeedsOutput(judgeType) && !isFile(problem + "/" + *it + ".out")) {
				cout << "No se encuentra la salida experada para el caso " << *it << endl;
				hasError = true;
			}
		}
		while (!toErase.empty()) {
			testCases.erase(toErase.top());
			toErase.pop();
		}
		testCases.sort();
		testCases.unique();
	}
	if (hasError) return 1;

	// Genera lista de codigos fuente
	hasError = false;
	if (sourceFiles.size() == 1 && isDir(*sourceFiles.begin())) {
		sourceFiles = Config::getDir(*sourceFiles.begin());
	}
	else {
		for (list<string>::iterator it = sourceFiles.begin(); it != sourceFiles.end(); it++) {
			if (!isFile(*it)) {
				cout << "No existe el archivo " << *it << endl;
				toErase.push(it);
			}
			else if (hasExtension(*it, "set")) {
				tmpList = Config::getSet(*it);
				sourceFiles.insert(sourceFiles.end(), tmpList.begin(), tmpList.end());
				toErase.push(it);
			}
		}
		while (!toErase.empty()) {
			sourceFiles.erase(toErase.top());
			toErase.pop();
		}
		sourceFiles.sort();
		sourceFiles.unique();
	}
	if (sourceFiles.size() == 0) {
		cout << "No hay casos por evaluar" << endl;
		hasError = true;
	}
	if (hasError) return 1;

	cout << "CASOS:" << endl;
	for (list<string>::iterator it = testCases.begin(); it != testCases.end(); it++) cout << *it << endl;
	cout << "FUENTES:" << endl;
	for (list<string>::iterator it = sourceFiles.begin(); it != sourceFiles.end(); it++) cout << *it << endl;

	/*cout << "La acción es " << action << " y el problema es " << problem << endl;
	cout << endl;
	cout << "Se compilará el archivo \"" << sourceFile << "\" en lenguaje " << lang << endl;
	cout << "El archivo debe pesar máximo " << maxSourceSize << "kB" << endl;
	cout << endl;
	cout << "Tiene " << maxCompTime << "ms y " << maxCompMem << "kB para que se compile" << endl;
	cout << "Durante su ejecución no podrá generar más de " << maxOutSize << "kB de salida" << endl;
	cout << endl;
	cout << "Se eveluará con el juez " << judgeType << " (ejecutable " << judgeExe << ")" << endl;
	cout << "Tiene " << maxRunTime << "ms y " << maxRunMem << "kB para la ejecución de cada caso" << endl;
	cout << "La salida " << (compareWhite?"":"NO ") << "tamará en cuenta espacios en blanco" << endl;
	cout << "La calificación será " << (strictEval?"todo o nada":"normal") << endl;
	cout << endl;
	cout << "¿Mostrar información extra? " << (verbose?"Sí":"No") << endl;
	cout << endl;*/

	///////////////////////////////////////////////////////////////////////////
	///////////// Rafael, se encarga de compilacion y ejecucion////////////////
	///////////////////////////////////////////////////////////////////////////
	#ifdef COMPREADY
	int programa;
	pid_t pID;
	int status;
	//int fd_pipe[2];
	//char buffer[128];
	//int leidos;

	pID = fork();

	//TODO compilar multiples programas

	if(pID == 0){
		//es el hijo, se encarga de la compilacion

		freopen("log.log", "w", stdout);
		freopen("error.log", "w", stderr);

		struct rlimit limit;

		//TODO: Establecer correctamente los limites de compilacion y hacer pruebas con Java, especialmente.

		//limit.rlim_cur = limit.rlim_max = maxCompTime * 1000;
		//setrlimit(RLIMIT_RTTIME, &limit);
		//limit.rlim_cur = limit.rlim_max = 64 * 1024 * 1024;
		//setrlimit(RLIMIT_AS, &limit);

		if(lang == "c"){
			programa = execlp("gcc","gcc", sourceFile.c_str(),"-o","out","-lm","-Wall",NULL);
		}

		else if (lang == "c++"){
			programa = execlp("g++","g++", sourceFile.c_str(),"-o","out","-lm","-Wall",NULL);
		}

		else{
			programa = execlp("javac","javac", sourceFile.c_str(),"out" ,NULL);
		}

		if(programa==-1){
			cout<<"No se pudo ejecutar el compilador"<<endl;
		}

	}

	else{
		//es el padre

		waitpid(pID, &status, 0);

		bool exito =false;

		if (WIFEXITED(status)) {
			if (WEXITSTATUS(status) == 0) {
				exito = true;
				cout << "El archivo fue compilado correctamente, ahora se procede a evaluar"<<endl;
			}
			else cout << "Error de compilación" << endl;
		} else if (WIFSIGNALED(status)) {
			cout << "Terminado con la señal" << WTERMSIG(status)<< endl;
		} else if (WIFSTOPPED(status)) {
			cout << "Detenido por la señal" << WSTOPSIG(status) << endl;
		} else if (WIFCONTINUED(status)) {
			cout << "wtf" << endl;
		}

		if(exito){
			//el programa ya fue compilado y esta listo poara ejecutarse



		}
	}


//	list<string>::iterator it;
//
//	for(it = testCases.begin(); it != testCases.end(); it++){
//		cout << "Compilando " << *it << endl;
//	}
	#endif
    return 0;
}
