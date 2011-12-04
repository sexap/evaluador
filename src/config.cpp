#include "config.h"

//TODO: Mejorar mensajes de error. (prioridad baja)
//TODO: Corregir fallos con variables de posición fija.
//TODO: Implementar BOOL y LIST de posición fija  o bien generar exepciones adecuadas.
//TODO: Implementar listas desde archivos. (Opcional)


namespace seap_implement {

	Config::Config() {
		sourceId = 0;
		hasFailed = false;
	}

	Config::~Config() {
		map<string, Data>::iterator it;

		for (it = variables.begin(); it != variables.end(); it++) {
			if(it->second.type == T_STRING) delete it->second.v_string;
			else if (it->second.type == T_LIST) delete it->second.v_list;
		}
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

	//////////////
	//  PARSEO  //
	//////////////
	bool Config::parseArgs(int argc, char* argv[]) {
		enum {W_ID, W_INT, W_STR, W_LST};
		int k, status = W_ID;
		Data* dest = NULL;
		map<string,Data>::iterator it;
		map<int,Data*>::iterator f_it;
		bool error = false;

		sourceId ++;

		for (k = 1; k < argc; k++) {
			// Busca identicadores y verifica su validez.
			if (status == W_ID) {
				//Verifica si es una variable fixed
				f_it = fixedVariables.find(k);
				if (f_it != fixedVariables.end()) {
					dest = f_it->second;
					dest->lastSeenBy = sourceId;

					if (dest->type == T_INT) {
						status = W_INT;
						k -= 1;
					}
					else if (dest->type == T_STRING) {
						status = W_STR;
						k -= 1;
					}
					else if (dest->type == T_BOOL) cout << "AVISO. Booleanos fixed no soportados" << endl;
					else if (dest->type == T_LIST) cout << "AVISO. Listas fixed no soportadas" << endl;
				}
				// Es una variable con nombre
				else if (!isValidId(argv[k])) {
					cout << "Se esperaba un id en vez de " << argv[k] << endl;
					error = true;
				}
				else {
					it = variables.find(argv[k] + 1);
					if (it == variables.end()) {
						cout << "No existe el id -" << argv[k] << endl;
						error = true;
					}
					else {
						dest = &(it->second);

						if (dest->source != S_ARG && dest->source != S_BOTH) {
							cout << "El id " << argv[k] << " se debe especificar en archivo" << endl;
						}
						else if (dest->fixedPos > 0) {
							cout << "El id " << argv[k] << " debe ir directamente en pos " << dest->fixedPos << endl;
						}
						else {
							if (dest->lastSeenBy == sourceId) {
								cout << "Se repite el parámetro " << argv[k] << endl;
								error = true;
							}
							else {
								dest->lastSeenBy = sourceId;
							}

							if (dest->type == T_BOOL) {
								dest->v_bool = !(dest->v_bool);
								dest->isSet = true;
							}
							else if (dest->type == T_INT) status = W_INT;
							else if (dest->type == T_STRING) status = W_STR;
							else if (dest->type == T_LIST) status = W_LST;
						}
					}
				}
			}
			// Se espera un valor entero.
			else if (status == W_INT) {
				if (isValidId(argv[k])) {
					cout << "Se esperaba un valor entero después de " << argv[k-1] << endl;
					error = true;
					if (dest->fixedPos <= 0) k -= 1;
				}
				else if (!isValidInt(argv[k])) {
					cout << "Se esperaba un entero en vez de " << argv[k] << endl;
					error = true;
				}
				else {
					dest->v_int = atoi(argv[k]);
					dest->isSet = true;
				}
				status = W_ID;
			}
			// Se espera una cadena.
			else if (status == W_STR) {
				if (isValidId(argv[k])) {
					cout << "Se esperaba un valor de cadena después de " << argv[k-1] << endl;
					error = true;
					if (dest->fixedPos <= 0) k -= 1;
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
					if (dest->fixedPos <= 0) k -= 1;
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

		hasFailed = hasFailed || error;
		return !error;
	} // Fin parseArgs()

	bool Config::parseFile(const string& filename) {
		using namespace libconfig;

		libconfig::Config config;
		libconfig::Setting::Type settingType;
		map<string, Data>::iterator it;
		Data* dest = NULL;
		int k, total;
		const char* varName;
		bool error = false;

		sourceId ++;

		// Abrir y parsear el archivo.
		try {
			config.readFile(filename.c_str());
		}
		catch (FileIOException& e) {
			cout << "No se pudo abrir el archivo " << filename << endl;
			return false;
		}
		catch (ParseException& e) {
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
					dest->lastSeenBy = sourceId;

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

		hasFailed = hasFailed || error;
		return !error;
	} // Fin parseFile()

	bool Config::validate() {
		map<string, Data>::iterator it;
		bool error = false;

		for (it = variables.begin(); it != variables.end(); it++) {
			if (it->second.isMandatory && it->second.lastSeenBy == 0) {
				error = true;
				if(it->second.source == S_ARG) {
					if (it->second.fixedPos > 0) {
						cout << "El argumento en la posición " << it->second.fixedPos << " es obligatorio" << endl;
					}
					else {
						cout << "El argumento -" << (it->first) << " es obligatorio" << endl;
					}
				}
				else if(it->second.source == S_FILE) {
					cout << "La variable " << (it->first) << " es obligatoria" << endl;
				}
				else {
					cout << "Debe especificarse la variable " << (it->first) << " o el argumento -" << (it->first) << endl;
				}
			}
		}

		return !(hasFailed || error);
	}

	////////////////////////////////
	//  TRATAMIENTO DE VARIABLES  //
	////////////////////////////////
	void Config::registerVariable(const string& name, Type type, int position, bool mandatory, Source source) {
		Data tmpData;

		variables.erase(name);

		tmpData.type = type;
		tmpData.source = source;
		tmpData.isMandatory = mandatory;
		tmpData.isSet = false;
		tmpData.lastSeenBy = 0;
		tmpData.fixedPos = position;

		if (type == T_STRING) tmpData.v_string = NULL;
		else if (type == T_LIST) tmpData.v_list = NULL;

		variables[name] = tmpData;
		if (position > 0) {
			fixedVariables[position] = &variables[name];
		}
	}

	void Config::registerArgVar(const string& name, Type type, int position, bool mandatory) {
		registerVariable(name, type, position, mandatory, S_ARG);
	}

	void Config::registerArgVar(const string& name, Type type, bool mandatory) {
		registerVariable(name, type, -1, mandatory, S_ARG);
	}

	void Config::registerFileVar(const string& name, Type type, bool mandatory) {
		registerVariable(name, type, -1, mandatory, S_FILE);
	}

	void Config::registerVar(const string& name, Type type, bool mandatory) {
		registerVariable(name, type, -1, mandatory, S_BOTH);
	}

	///////////////
	//  SETTERS  //
	///////////////
	void Config::setValue(const string& name, bool value) {
		map<string, Data>::iterator it = variables.find(name);
		if (it == variables.end()) throw ConfigExceptionNotFound();
		if (it->second.type != T_BOOL) throw ConfigExceptionBadType();
		it->second.v_bool = value;
	}

	void Config::setValue(const string& name, int value) {
		map<string, Data>::iterator it = variables.find(name);
		if (it == variables.end()) throw ConfigExceptionNotFound();
		if (it->second.type != T_INT) throw ConfigExceptionBadType();
		it->second.v_int = value;
	}

	void Config::setValue(const string& name, const string& value) {
		map<string, Data>::iterator it = variables.find(name);
		if (it == variables.end()) throw ConfigExceptionNotFound();
		if (it->second.type != T_STRING) throw ConfigExceptionBadType();
		delete it->second.v_string;
		it->second.v_string = new string(value);
	}

	void Config::setValue(const string& name, const char* value) {
		setValue(name, string(value));
	}

	///////////////
	//  GETTERS  //
	///////////////
	void Config::getValue(const string& name, bool& var) {
		map<string, Data>::iterator it = variables.find(name);
		if (it == variables.end()) throw ConfigExceptionNotFound();
		if (it->second.type != T_BOOL) throw ConfigExceptionBadType();
		var = it->second.v_bool;
	}

	void Config::getValue(const string& name, int& var) {
		map<string, Data>::iterator it = variables.find(name);
		if (it == variables.end()) throw ConfigExceptionNotFound();
		if (it->second.type != T_INT) throw ConfigExceptionBadType();
		var = it->second.v_int;
	}

	void Config::getValue(const string& name, string& var) {
		map<string, Data>::iterator it = variables.find(name);
		if (it == variables.end()) throw ConfigExceptionNotFound();
		if (it->second.type != T_STRING) throw ConfigExceptionBadType();
		if (it->second.v_string == NULL) throw ConfigExceptionNoData();
		var = *(it->second.v_string);
	}

	void Config::getValue(const string& name, list<string>& var) {
		map<string, Data>::iterator it = variables.find(name);
		if (it == variables.end()) throw ConfigExceptionNotFound();
		if (it->second.type != T_LIST) throw ConfigExceptionBadType();
		if (it->second.v_list == NULL) var = list<string>();
		else var = *(it->second.v_list);
	}

	///////////////////////////////
	//  CONJUNTOS Y DIRECTORIOS  //
	///////////////////////////////
	list<string> Config::getSet(const string& path){
		string line;
		ifstream file;
		list<string> result;

		file.open(path.c_str());
		if(file.fail()) return result;

		while(getline(file, line)) {
			if (line != "") result.push_back(line);
		}
		return result;
	}

	list<string> Config::getDir(const string& path, const string& ext) {
		list<string> result;
		string filename;
		size_t p;

		DIR *dirp;
		struct dirent *dp;

		if ((dirp = opendir(path.c_str())) == NULL) {
			return result;
		}

		while ((dp = readdir(dirp)) != NULL) {
			if(dp->d_name[0] != '.') {
				filename = dp->d_name;
				p = filename.rfind('.');
				if (ext == "" || (p != string::npos && filename.substr(p) == "." + ext)) result.push_back(dp->d_name);
			}
		}
		return result;
	}
}
