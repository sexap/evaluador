#include "ParamParser.h"

ParamParser::ParamParser() {
	// Iniciar valores por default
	maxCompileTime = 30000;
	maxCompileMem = 1024;
	maxSourceFileSize = 1024;
	verbose = false;
	strictEval = false;
	strictWhitespace = false;

	// Indica los parametros obligatorios y opcionales
}

bool ParamParser::isValidFile(const char* str) {
	return true;
}

bool ParamParser::isValidLang(const char* str) {
	return strcmp(str, "c") == 0 || strcmp(str, "cpp") == 0 || strcmp(str, "java") == 0;
}

bool ParamParser::isValidNumber(const char* str) {
	int tam = strlen(str);
	int p;
	bool valid = true;

	if (tam == 0) return false;

	for (p = 0; p < tam; p++) {
		if (str[p] < '0' || str[p] > '9') {
			valid = false;
			break;
		}
	}

	return valid;
}

bool ParamParser::isValidJudge(const char* str) {
	return strcmp(str, "normal") == 0 || strcmp(str, "especial") == 0 || strcmp(str, "interactivo") == 0;
}

bool ParamParser::parse(int argc, char* argv[]) {
	int i;
	map<string, int>::iterator it;
	bool error = false;

	// Parsea los parámetros
	for (i = 1; i < argc; i++) {
		// -h
		if (strcmp(argv[i], "-h") == 0) {
			cerr <<
			"\n\t- SEAP: Sistema de Evaluación Automática de Programas -\n"
			"\nOPCIONES:\n"
			"-h\t\t Muestra este mensaje de ayuda.\n"
			"-f archivo\t Código fuente a evaluar.\n"
			"-l c|cpp|java\t Lenguaje del código fuente.\n"
			"-fz kibibytes\t Tamaño máximo permitido para el código fuente.\n"
			"-T milisegundos\t Tiempo máximo permitido para la compilación.\n"
			"-M kibibytes\t Memoria máxima permitida para la compilación.\n"
			"-FZ kibibytes\t Tamaño máximo de la salida del programa a evaluar.\n"
			"-tc archivos...\t Archivos de casos de prueba.\n"
			"-j normal|especial|interactivo\n"
			"\t\t Tipo de juez que evaluará el programa.\n"
			"-x ejecutable\t Ejecutable que evaluará la salida del programa.\n"
			"-e\t\t Evalúa en modo estricto.\n"
			"-w\t\t Evalúa comparando los espacios en blanco de manera estricta.\n"
			"-v [archivo]\t Muestra información extra acerca de la evaluación.\n"
			"\t\t Si se especifica un archivo se guarda la información en él.\n"
			<< flush;
		}
		// -f
		else if (strcmp(argv[i], "-f") == 0) {
			if (i+1 < argc) {
				if (isValidFile(argv[i+1])) {
					sourceFile = argv[i+1];
				}
				else {
					cerr << "No se puede abrir " << argv[i+1] << endl;
					error = true;
				}
				i++;
			}
			else {
				cerr << "Se esperaba un nombre de archivo después de -f" << endl;
				error = true;
			}
		}
		// -l
		else if (strcmp(argv[i], "-l") == 0) {
			if (i+1 < argc) {
				if (isValidLang(argv[i+1])) {
					language = argv[i+1];
				}
				else {
					cerr << argv[i+1] << " no es un lenguaje válido" << endl;
					error = true;
				}
				i++;
			}
			else {
				cerr << "Se esperaba un lenguaje después de -l" << endl;
				error = true;
			}
		}
		// -fz
		else if (strcmp(argv[i], "-fz") == 0) {
			if (i+1 < argc) {
				if (isValidNumber(argv[i+1])) {
					maxSourceFileSize = atoi(argv[i+1]);
				}
				else {
					cerr << argv[i+1] << " no es numero valido" << endl;
					error = true;
				}
				i++;
			}
			else {
				cerr << "Se esperaba un numero después de -fz" << endl;
				error = true;
			}
		}

		// -e
		else if (strcmp(argv[i], "-e") == 0) {
			strictEval = true;
		}
		// -w
		else if (strcmp(argv[i], "-w") == 0) {
			strictWhitespace = true;
		}
		// -v
		else if (strcmp(argv[i], "-v") == 0) {
			verbose = true;
		}
		// param desconocido
		else {
			cerr << "Parámetro " << argv[i] << " desconocido" << endl;
			error = true;
		}
	}

	if (error) cerr << "Escriba '" << argv[0] << " -h' para obtener ayuda" << endl;
	return !error;
}
