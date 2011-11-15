#include <iostream>
using namespace std;

#include "config.h"
using namespace seap_implement;

int main(int argc, char* argv[]) {
	Config cfg;
	string sourceFile, lang, jType, jExe;
	int fileSize, compTime, compMem, outSize, runTime, runMem;
	bool verbose, strict, iWhite;

	// Registra variables
	cfg.registerVariable("f", Config::T_STRING, true, Config::S_ARG);
	cfg.registerVariable("l", Config::T_STRING, false, Config::S_ARG);
	cfg.registerVariable("fz", Config::T_INT, false, Config::S_ARG);
	cfg.registerVariable("T", Config::T_INT, false, Config::S_ARG);
	cfg.registerVariable("M", Config::T_INT, false, Config::S_ARG);
	cfg.registerVariable("OZ", Config::T_INT, false, Config::S_ARG);
	cfg.registerVariable("tc", Config::T_LIST, false, Config::S_ARG);
	cfg.registerVariable("v", Config::T_BOOL, false, Config::S_ARG);

	cfg.registerVariable("max_time", Config::T_INT, true, Config::S_FILE);
	cfg.registerVariable("max_mem", Config::T_INT, false, Config::S_FILE);
	cfg.registerVariable("judge_type", Config::T_STRING, true, Config::S_FILE);
	cfg.registerVariable("judge_exe", Config::T_STRING, false, Config::S_FILE);
	cfg.registerVariable("strict_eval", Config::T_BOOL, false, Config::S_FILE);
	cfg.registerVariable("ignore_white", Config::T_BOOL, false, Config::S_FILE);

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
		return 0;
	}

	// Ya están los valores
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

	cout << "¿Mostrar información extra? " << boolalpha << verbose << endl;
	cout << endl;
	cout << "Se compilará el archivo \"" << sourceFile << "\" en lenguaje " << lang << endl;
	cout << "El archivo debe pesar máximo " << fileSize << "kB" << endl;
	cout << endl;
	cout << "Tiene " << compTime << "ms y " << compMem << "kB para que se compile" << endl;
	cout << "Durante su ejecución no podrá generar más de " << outSize << "kB de salida" << endl;
	cout << endl;
	cout << "Se eveluará con el juez " << jType << " (ejecutable " << jExe << ")" << endl;
	cout << "Tiene " << runTime << "ms y " << runMem << "kB para su ejecución" << endl;
	cout << "La salida " << (iWhite?"":"NO ") << "ignorará espacios en blanco" << endl;
	cout << "La calificación será " << (strict?"todo o nada":"normal") << endl;

    return 0;
}
