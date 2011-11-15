#include "config.h"

//TODO: Implementar listas desde archivos.
//TODO: Implemenar el destructor, liberando cadenas y listas.

namespace seap_implement {
	Config::Config() {
	}

	Config::~Config() {
	}

	////////////////////
	//  VALIDACIONES  //
	////////////////////
	bool Config::isValidId(const char* s) {
		return s[0] == '-';
	}

	bool Config::isValidInt(const char* s) {
		int i;

		for (i = 0; s[i]; i++) {
			if (s[i] < '0' || s[i] > '9') return false;
		}
		return true;
	}

	////////////////////////////////
	//  TRATAMIENTO DE VARIABLES  //
	////////////////////////////////
	void Config::registerVariable(string name, Type type, bool mandatory, Source source) {
		Data tmpData;

		tmpData.type = type;
		tmpData.source = source;
		tmpData.isMandatory = mandatory;
		tmpData.isSet = false;
		tmpData.timesSeen = 0;

		if (type == T_STRING) tmpData.v_string = NULL;
		else if (type == T_LIST) tmpData.v_list = NULL;

		variables[name] = tmpData;
	}

	bool Config::parseArgs(int argc, char* argv[], int begin) {
		enum {W_ID, W_INT, W_STR, W_LST};
		int k, status = W_ID;
		Data* dest = NULL;
		map<string, Data>::iterator it;
		bool error = false;

		for (k = begin; k < argc; k++) {
			// Busca identicadores y verifica su validez.
			if (status == W_ID) {
				if (isValidId(argv[k])) {
					it = variables.find(argv[k] + 1);
					if (it == variables.end()) {
						cout << "No existe el id " << argv[k] << endl;
						error = true;
					}
					else {
						dest = &(it->second);

						if (dest->source == S_ARG || dest->source == S_BOTH) {
							dest->timesSeen += 1;
							if (dest->timesSeen > 1) {
								cout << "Se repite el parámetro " << argv[k] << endl;
								error = true;
							}

							if (dest->type == T_BOOL) {
								dest->v_bool = !(dest->v_bool);
								dest->isSet = true;
							}
							else if (dest->type == T_INT) status = W_INT;
							else if (dest->type == T_STRING) status = W_STR;
							else if (dest->type == T_LIST) status = W_LST;
						}
						else {
							cout << "El id " << argv[k] << " se debe especificar en archivo" << endl;
						}
					}
				}
				else {
					cout << "Se esperaba un id en vez de " << argv[k] << endl;
					error = true;
				}
			}
			// Se espera un valor entero.
			else if (status == W_INT) {
				if (isValidId(argv[k])) {
					cout << "Se esperaba un valor entero después de " << argv[k-1] << endl;
					error = true;
					k -= 1;
				}
				else if (isValidInt(argv[k])) {
					dest->v_int = atoi(argv[k]);
					dest->isSet = true;
				}
				else {
					cout << "Se esperaba un entero en vez de " << argv[k] << endl;
					error = true;
				}
				status = W_ID;
			}
			// Se espera una cadena.
			else if (status == W_STR) {
				if (isValidId(argv[k])) {
					cout << "Se esperaba un valor de cadena después de " << argv[k-1] << endl;
					error = true;
					k -= 1;
				}
				else {
					delete dest->v_string;
					dest->v_string = new string(argv[k]);
					dest->isSet = true;
				}
				status = W_ID;
			}
			// Se espera una lista.
			else if (status == W_LST) {
				if (isValidId(argv[k])) {
					if (dest->v_list == NULL) {
						cout << "Se esperaba al menos un elemento después de " << argv[k-1] << endl;
						error = true;
					}
					status = W_ID;
					k -= 1;
				}
				else {
					if(!(dest->isSet)) {
						dest->v_list = new list<string>();
						dest->v_list->push_back(argv[k]);
						dest->listIt = dest->v_list->begin();
						dest->isSet = true;
					}
					else {
						dest->v_list->push_back(argv[k]);
						dest->listIt = dest->v_list->begin();
					}
				}
			}
		} // Fin for

		// Se verifica el estado final.
		if (status == W_INT) {
			cout << "Se esperaba un valor entero después de " << argv[k-1] << endl;
			error = true;
		}
		else if (status == W_STR) {
			cout << "Se esperaba un valor de cadena después de " << argv[k-1] << endl;
			error = true;
		}
		else if (status == W_LST && dest->v_list == NULL) {
			cout << "Se esperaba al menos un elemento después de " << argv[k-1] << endl;
			error = true;
		}

		return !error;
	} // Fin parseArgs()

	bool Config::parseFile(const char* filename) {
		using namespace libconfig;

		libconfig::Config config;
		libconfig::Setting::Type settingType;
		map<string, Data>::iterator it;
		Data* dest = NULL;
		int k, total;
		const char* varName;
		bool error = false;

		// Abrir y parsear el archivo.
		try {
			config.readFile(filename);
		}
		catch (FileIOException e) {
			cout << "No se pudo abrir el archivo " << filename << endl;
			return false;
		}
		catch (ParseException e) {
			cout << filename << ":" << e.getLine() << " ";
			cout << "Archivo " << filename << " mal formado" << endl;
			return false;
		}

		// Analizar las variables
		libconfig::Setting& root = config.getRoot();
		total = root.getLength();
		for (k = 0; k < total; k++) {
			varName = root[k].getName();
			it = variables.find(varName);

			if (it == variables.end()) {
				cout << filename << ":" << root[k].getSourceLine() << " ";
				cout << "No existe la variable " << varName << endl;
				error = true;
			}
			else {
				dest = &(it->second);

				if (dest->source != S_FILE && dest->source != S_BOTH) {
					cout << filename << ":" << root[k].getSourceLine() << " ";
					cout << "La variable " << root[k].getName() << " se debe especificar como parámetro" << endl;
					error = true;
				}
				else {
					settingType = root[k].getType();

					// Verifica tipos y realiza asignación.
					if (dest->type == T_BOOL) {
						if (settingType != Setting::TypeBoolean) {
							cout << filename << ":" << root[k].getSourceLine() << " ";
							cout << "Se esperaba un valor booleano para " << varName << endl;
							error = true;
						}
						else {
							dest->v_bool = root[k];
							dest->isSet = true;
						}
					}
					else if (dest->type == T_INT) {
						if (settingType != Setting::TypeInt) {
							cout << filename << ":" << root[k].getSourceLine() << " ";
							cout << "Se esperaba un valor entero para " << varName << endl;
							error = true;
						}
						else {
							dest->v_int = root[k];
							dest->isSet = true;
						}
					}
					else if (dest->type == T_STRING) {
						if (settingType != Setting::TypeString) {
							cout << filename << ":" << root[k].getSourceLine() << " ";
							cout << "Se esperaba un valor de cadena para " << varName << endl;
							error = true;
						}
						else {
							delete dest->v_string;
							dest->v_string = new string(root[k].c_str());
							dest->isSet = true;
						}
					}
					else if (dest->type == T_LIST) {
						cout << filename << ":" << root[k].getSourceLine() << " ";
						cout << "AVISO. Listas en archivos no soportadas ¿aún?" << endl;
					}
				}
			}
		}

		return !error;
	} // Fin parseFile()

	bool Config::validate() {
		map<string, Data>::iterator it;
		bool error = false;

		for (it = variables.begin(); it != variables.end(); it++) {
			if (it->second.isMandatory && !it->second.isSet) {
				error = true;
				if (it->second.timesSeen == 0) {
					if(it->second.source == S_ARG) {
						cout << "El argumento -" << (it->first) << " es obligatorio" << endl;
					}
					else if(it->second.source == S_FILE) {
						cout << "La variable " << (it->first) << " es obligatoria" << endl;
					}
					else {
						cout << "Debe especificarse la variable " << (it->first) << " o el argumento -" << (it->first) << endl;
					}
				}
			}
		}
		return !error;
	}

	///////////////
	//  SETTERS  //
	///////////////
	Config::Status Config::setValue(string name, bool value) {
		map<string, Data>::iterator it = variables.find(name);
		if (it == variables.end()) return NOTFOUND;
		if (it->second.type != T_BOOL) return BADTYPE;
		it->second.v_bool = value;
		return SUCCESS;
	}

	Config::Status Config::setValue(string name, int value) {
		map<string, Data>::iterator it = variables.find(name);
		if (it == variables.end()) return NOTFOUND;
		if (it->second.type != T_INT) return BADTYPE;
		it->second.v_int = value;
		return SUCCESS;
	}

	Config::Status Config::setValue(string name, const char* value) {
		return setValue(name, string(value));
	}

	Config::Status Config::setValue(string name, string value) {
		map<string, Data>::iterator it = variables.find(name);
		if (it == variables.end()) return NOTFOUND;
		if (it->second.type != T_STRING) return BADTYPE;
		delete it->second.v_string;
		it->second.v_string = new string(value);
		return SUCCESS;
	}

	///////////////
	//  GETTERS  //
	///////////////
	Config::Status Config::getValue(string name, bool& var) {
		map<string, Data>::iterator it;

		it = variables.find(name);
		if (it == variables.end()) return NOTFOUND;
		if (it->second.type != T_BOOL) return BADTYPE;
		var = it->second.v_bool;
		return (it->second.isSet)?SUCCESS:NOTSET;
	}

	Config::Status Config::getValue(string name, int& var) {
		map<string, Data>::iterator it;

		it = variables.find(name);
		if (it == variables.end()) return NOTFOUND;
		if (it->second.type != T_INT) return BADTYPE;
		var = it->second.v_int;
		return (it->second.isSet)?SUCCESS:NOTSET;
	}

	Config::Status Config::getValue(string name, string& var) {
		map<string, Data>::iterator it;

		it = variables.find(name);
		if (it == variables.end()) return NOTFOUND;
		if (it->second.type != T_STRING) return BADTYPE;
		if (it->second.v_string == NULL) return ERROR;
		var = *(it->second.v_string);
		return (it->second.isSet)?SUCCESS:NOTSET;
	}

	bool Config::hasMoreItems(string name) {
		map<string, Data>::iterator it;

		it = variables.find(name);
		if (it == variables.end()) return false;
		if (it->second.type != T_LIST) return false;
		if (it->second.v_list == NULL) return false;
		return (it->second.listIt != it->second.v_list->end());
	}

	Config::Status Config::getNextItem(string name, string& var) {
		map<string, Data>::iterator it;

		it = variables.find(name);
		if (it == variables.end()) return NOTFOUND;
		if (it->second.type != T_LIST) return BADTYPE;
		if (it->second.v_list == NULL) return ERROR;
		var = *(it->second.listIt);
		it->second.listIt ++;
		return SUCCESS;
	}
}
