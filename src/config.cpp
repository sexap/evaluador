#include "config.h"

//TODO: Implementar BOOL y LIST de posición fija  o generar exepciones adecuadas. (Opcional)
//TODO: Implementar LIST desde archivos o generar excepciones adecuadas. (Opcional)


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
		list<Data*>::iterator f_it;
		bool error = false;

		sourceId ++;

		for (k = 1, f_it = fixedVariables.begin(); f_it != fixedVariables.end(); k++, f_it++) {

			dest = (*f_it);

			if (k == argc) {
				cerr << "Se esperaba un argumento después de " << argv[k-1] << endl;
				error = true;
				break;
			}

			dest->lastSeenBy = sourceId;

			if (dest->type == T_INT) {
				// Es de tipo entero
				if (isValidId(argv[k]) || !isValidInt(argv[k])) {
					cerr << "Se esperaba un entero en vez de " << argv[k] << endl;
					error = true;
				}
				else {
					dest->v_int = atoi(argv[k]);
					dest->isSet = true;
				}
			}
			else if (dest->type == T_STRING) {
				if (isValidId(argv[k])) {
					cerr << "Se esperaba un valor de cadena en vez de " << argv[k] << endl;
					error = true;
				}
				else {
					delete dest->v_string;
					dest->v_string = new string(argv[k]);
					dest->isSet = true;
				}
			}
			else if (dest->type == T_BOOL) throw ConfigExceptionNotImplemented();
			else if (dest->type == T_LIST) throw ConfigExceptionNotImplemented();
		}

		for ( ; k < argc; k++) {
			// Busca identicadores y verifica su validez.
			if (status == W_ID) {
				// Es una variable con nombre
				if (!isValidId(argv[k])) {
					cerr << "Se esperaba una opción en vez de " << argv[k] << endl;
					error = true;
				}
				else {
					it = variables.find(argv[k] + 1);
					if (it == variables.end() || it->second.fixedPos || it->second.source == S_FILE) {
						cerr << "No existe el la opción " << argv[k] << endl;
						error = true;
					}
					else {
						dest = &(it->second);

						if (dest->lastSeenBy == sourceId) {
							cerr << "La opción " << argv[k] << "está repetida" << endl;
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
			// Se espera un valor entero.
			else if (status == W_INT) {
				if (isValidId(argv[k])) {
					cerr << "Se esperaba un valor entero después de " << argv[k-1] << endl;
					error = true;
					k -= 1;
				}
				else if (!isValidInt(argv[k])) {
					cerr << "Se esperaba un entero en vez de " << argv[k] << endl;
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
					cerr << "Se esperaba un valor de cadena después de " << argv[k-1] << endl;
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
						cerr << "Se esperaba al menos un elemento después de " << argv[k-1] << endl;
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
			cerr << "Se esperaba un valor entero después de " << argv[k-1] << endl;
			error = true;
		}
		else if (status == W_STR) {
			cerr << "Se esperaba un valor de cadena después de " << argv[k-1] << endl;
			error = true;
		}
		else if (status == W_LST && dest->v_list == NULL) {
			cerr << "Se esperaba al menos un elemento después de " << argv[k-1] << endl;
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
			cerr << "No se pudo abrir el archivo " << filename << endl;
			return false;
		}
		catch (ParseException& e) {
			cerr << "Error en el archivo '" << filename << "' línea " << e.getLine() << endl;
			cerr << "  Archivo mal formado" << endl;
			return false;
		}

		// Analizar las variables
		libconfig::Setting& root = config.getRoot();
		total = root.getLength();
		for (k = 0; k < total; k++) {
			varName = root[k].getName();
			it = variables.find(varName);

			if (it == variables.end() || it->second.source == S_ARG) {
				cerr << "Error en el archivo '" << filename << "' línea " << root[k].getSourceLine() << endl;
				cerr << "  No existe la opción " << varName << endl;
				error = true;
			}
			else {
				dest = &(it->second);

				settingType = root[k].getType();
				dest->lastSeenBy = sourceId;

				// Verifica tipos y realiza asignación.
				if (dest->type == T_BOOL) {
					if (settingType != Setting::TypeBoolean) {
						cerr << "Error en el archivo '" << filename << "' línea " << root[k].getSourceLine() << endl;
						cerr << "  Se esperaba true o false para la opción " << varName << endl;
						error = true;
					}
					else {
						dest->v_bool = root[k];
						dest->isSet = true;
					}
				}
				else if (dest->type == T_INT) {
					if (settingType != Setting::TypeInt) {
						cerr << "Error en el archivo '" << filename << "' línea " << root[k].getSourceLine() << endl;
						cerr << "  Se esperaba un número entero para la opción " << varName << endl;
						error = true;
					}
					else {
						dest->v_int = root[k];
						dest->isSet = true;
					}
				}
				else if (dest->type == T_STRING) {
					if (settingType != Setting::TypeString) {
						cerr << "Error en el archivo '" << filename << "' línea " << root[k].getSourceLine() << endl;
						cerr << "  Se esperaba una cadena para la opción " << varName << endl;
						error = true;
					}
					else {
						delete dest->v_string;
						dest->v_string = new string(root[k].c_str());
						dest->isSet = true;
					}
				}
				else if (dest->type == T_LIST) throw ConfigExceptionNotImplemented();
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
					if (it->second.fixedPos) {
						cerr << "La opción " << (it->first) << " es obligatoria" << endl;
					}
					else {
						cerr << "La opción -" << (it->first) << " es obligatoria" << endl;
					}
				}
				else if(it->second.source == S_FILE) {
					cerr << "La opción " << (it->first) << " es obligatoria" << endl;
					cerr << "  Debe especificarse en un archivo de configuración." << endl;
				}
				else {
					// Este error no se usa en realidad
					cerr << "La opción " << (it->first) << " es obligatoria" << endl;
					cerr << "  Debe especificarse en los parámetros del programa o en un archivo de configuración." << endl;
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
		tmpData.fixedPos = (position > 0);

		if (type == T_STRING) tmpData.v_string = NULL;
		else if (type == T_LIST) tmpData.v_list = NULL;

		if (position > 0) {
			variables[name] = tmpData;
			fixedVariables.push_back(&variables[name]);
		}
		else {
			variables[name] = tmpData;
		}
	}

	void Config::registerArgVar(const string& name, Type type, bool mandatory) {
		registerVariable(name, type, -1, mandatory, S_ARG);
	}

	void Config::registerArgFixVar(const string& name, Type type) {
		registerVariable(name, type, 1, true, S_ARG);
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
			if(dp->d_type == DT_REG && dp->d_name[0] != '.') {
				filename = dp->d_name;
				p = filename.rfind('.');
				if (ext == "" || (p != string::npos && filename.substr(p) == "." + ext)) result.push_back(dp->d_name);
			}
		}
		return result;
	}
}
