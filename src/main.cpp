#include <iostream>
using namespace std;

#include "config.h"
#include "validation.h"
using namespace seap_implement;

int main(int argc, char* argv[]) {
	Config cfg;
	string sourceFile, lang, jType, jExe, caseFile, action, problem;
	int fileSize, compTime, compMem, outSize, runTime, runMem;
	bool verbose, strict, iWhite;

	// Registra variables
	cfg.registerArgVar("action", Config::T_STRING, 1);
	cfg.registerArgVar("problem", Config::T_STRING, 2);

	cfg.registerArgVar("f", Config::T_STRING, true);
	cfg.registerArgVar("l", Config::T_STRING, false);
	cfg.registerArgVar("fz", Config::T_INT, false);
	cfg.registerArgVar("T", Config::T_INT, false);
	cfg.registerArgVar("M", Config::T_INT, false);
	cfg.registerArgVar("OZ", Config::T_INT, false);
	cfg.registerArgVar("tc", Config::T_LIST, false);
	cfg.registerArgVar("v", Config::T_BOOL, false);

	cfg.registerFileVar("max_time", Config::T_INT, true);
	cfg.registerFileVar("max_mem", Config::T_INT, false);
	cfg.registerFileVar("judge_type", Config::T_STRING, true);
	cfg.registerFileVar("judge_exe", Config::T_STRING, false);
	cfg.registerFileVar("strict_eval", Config::T_BOOL, false);
	cfg.registerFileVar("ignore_white", Config::T_BOOL, false);

	// Valores por default
	cfg.setValue("l", "auto"); // Lenguaje auto
	cfg.setValue("fz", 1024); // 1MB de código
	cfg.setValue("T", 15000); // 15s para compilar
	cfg.setValue("M", 4096); // 4MB para compilar
	cfg.setValue("OZ", 128); // 128KB de salida
	cfg.setValue("v", false); // Es callado

	cfg.setValue("max_mem", 4096); // 4MB para ser ejecutado
	cfg.setValue("judge_exe", "judge"); // Ejecutable del juez
	cfg.setValue("strict_eval", false); // No es estricto
	cfg.setValue("ignore_white", true); // Ignora espacios extra

	// Procesa argv[]
	cfg.parseFile("eval.conf");
	cfg.parseArgs(argc, argv);
	if (!cfg.validate()) {
		cout << "Consulte el manual" << endl;
		return 1;
	}

	// Ya están los valores
	cfg.getValue("action", action);
	cfg.getValue("problem", problem);

	cfg.getValue("f", sourceFile);
	cfg.getValue("l", lang);
	cfg.getValue("fz", fileSize);
	cfg.getValue("T" , compTime);
	cfg.getValue("M" , compMem);
	cfg.getValue("OZ" , outSize);
	cfg.getValue("v" , verbose);

	cfg.getValue("max_time", runTime);
	cfg.getValue("max_mem", runMem);
	cfg.getValue("judge_type", jType);
	cfg.getValue("judge_exe", jExe);
	cfg.getValue("strict_eval", strict);
	cfg.getValue("ignore_white", iWhite);

	// Automaticamente obtiene el lenguaje
	if (lang == "auto") {
		if(!forceValidLang(lang, sourceFile)) {
			cout << "No se reconoce el lenguaje del archivo " << sourceFile << endl;
			return 1;
		}
	}
	else if (!isValidLang(lang)) {
		cout << lang << " no es un lenguaje válido" << endl;
		return 1;
	}

	// Valida el tipo de juez
	if (!isValidJudgeType(jType)) {
		cout << jType << " no es un tipo de juez válido" << endl;
		return 1;
	}

	cout << "La acción es " << action << " y el problema es " << problem << endl;
	cout << endl;
	cout << "Se compilará el archivo \"" << sourceFile << "\" en lenguaje " << lang << endl;
	cout << "El archivo debe pesar máximo " << fileSize << "kB" << endl;
	cout << endl;
	cout << "Tiene " << compTime << "ms y " << compMem << "kB para que se compile" << endl;
	cout << "Durante su ejecución no podrá generar más de " << outSize << "kB de salida" << endl;
	cout << endl;
	cout << "Se eveluará con el juez " << jType << " (ejecutable " << jExe << ")" << endl;
	cout << "Tiene " << runTime << "ms y " << runMem << "kB para la ejecución de cada caso" << endl;
	cout << "La salida " << (iWhite?"":"NO ") << "ignorará espacios en blanco" << endl;
	cout << "La calificación será " << (strict?"todo o nada":"normal") << endl;
	cout << endl;
	cout << "¿Mostrar información extra? " << boolalpha << verbose << endl;
	cout << endl;
	cout << "Los casos son:" << endl;
	while(cfg.hasMoreItems("tc")) {
		cfg.getNextItem("tc", caseFile);
		cout << caseFile << endl;
	}

    return 0;
}
