#include <iostream>
#include <list>
#include <stack>
using namespace std;

#include "config.h"
#include "validation.h"
using namespace seap_implement;

int main(int argc, char* argv[]) {
	Config confArg, confFile;
	string sourceFile, lang, judgeType, judgeExe, action, problem;
	int maxSourceSize, maxCompTime, maxCompMem, maxOutSize, maxRunTime, maxRunMem;
	bool verbose, strictEval, compareWhite, hasError;
	list<string> testCases, tmpList;
	stack<list<string>::iterator> toErase;

	// Registra variables
	confArg.registerArgVar("action", Config::T_STRING, 1);
	confArg.registerArgVar("problem", Config::T_STRING, 2);

	confArg.registerArgVar("f", Config::T_STRING, true);
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
	confArg.setValue("s", 8); // 8kB de código
	confArg.setValue("T", 10000); // 10s para compilar
	confArg.setValue("M", 4096); // 4MB para compilar (revisar)
	confArg.setValue("S", 1); // 1KB de salida
	confArg.setValue("v", false); // Es callado

	confFile.setValue("max_mem", 4096); // 4MB para ser ejecutado (revisar)
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

	confArg.getValue("f", sourceFile);
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
	if (!isBetween(maxCompTime, 1, 60000)) {
		cout << "el parametro -T debe estar entre 1ms y 60s" << endl;
		hasError = true;
	}
	if (!isBetween(maxCompMem, 1, 262144)) {
		cout << "el parametro -M debe estar entre 1kB y 256MB" << endl;
		hasError = true;
	}
	if (!isBetween(maxOutSize, 1, 32)) {
		cout << "el parametro -S debe estar entre 1kB y 32kB" << endl;
		hasError = true;
	}
	if (!isBetween(maxSourceSize, 1, 512)) {
		cout << "el parametro -s debe estar entre 1kB y 512kB" << endl;
		hasError = true;
	}
	if (hasError) return 1;

	//TODO: Si se indica como archivo fuente una carpeta, usar todos los archivos de esa carpeta
	hasError = false;
	if (!isFile(sourceFile)) {
		cout << "No existe el archivo " << sourceFile << endl;
		hasError = true;
	}
	else {
		if (lang == "auto") {
			if(!forceValidLang(lang, sourceFile)) {
				cout << "No se reconoce el lenguaje del archivo " << sourceFile << endl;
				hasError = true;
			}
		}
		else if (!isValidLang(lang)) {
			cout << lang << " no es un lenguaje válido" << endl;
			hasError = true;
		}
		if (!isFileSmaller(sourceFile, maxSourceSize)) {
			cout << sourceFile << " es más grande que el límite establecido." << endl;
			hasError = true;
		}
	}
	if (hasError) return 1;

	// Analizar y valida archivo
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
	if (testCases.size() == 0) {
		testCases = Config::getDir(problem, "case");
		for (list<string>::iterator it = testCases.begin(); it != testCases.end(); it++) {
			*it = it->substr(0, it->rfind('.'));
		}
	}
	else {
		for (list<string>::iterator it = testCases.begin(); it != testCases.end(); it++) {
			if (hasExtension(*it, "set")) {
				tmpList = Config::getSet(problem + "/" + *it);
				testCases.insert(testCases.end(), tmpList.begin(), tmpList.end());
				toErase.push(it);
			}
		}
		while (!toErase.empty()) {
			testCases.erase(toErase.top());
			toErase.pop();
		}
		testCases.sort();
		testCases.unique();
	}

	// Verifica existencia de casos de prueba
	hasError = false;
	/*for (list<string>::iterator it = testCases.begin(); it != testCases.end(); it++) {
		if (!isFile(problem + "/" + caseName + ".case")) {
			cout << "No se encuentra el caso " << caseName << endl;
			hasError = true;
		}
		else if (judgeNeedsOutput(judgeType) && !isFile(problem + "/" + caseName + ".out")) {
			cout << "No se encuentra la salida experada para el caso " << caseName << endl;
			hasError = true;
		}
		else {
			testCases.push_back(problem + "/" + caseName + ".case");
		}
	}*/
	if (hasError) return 1;

	cout << "La acción es " << action << " y el problema es " << problem << endl;
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
	cout << endl;
	cout << "Los casos son:" << endl;
	/*while(confArg.hasMoreItems("c")) {
		confArg.getNextItem("c", caseName);
		cout << caseName << endl;
	}*/

    return 0;
}
