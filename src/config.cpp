#include "config.h"

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
	void Config::registerVariable(string name, Type type, bool mandatory) {
		Data tmpData;

		tmpData.type = type;
		tmpData.isMandatory = mandatory;
		tmpData.isSet = false;
		tmpData.timesSeen = 0;

		if (type == T_STRING) tmpData.v_string = NULL;
		else if (type == T_LIST) tmpData.v_list = NULL;

		variables[name] = tmpData;
	}

	bool Config::parseArgs(int argc, char* argv[], int begin) {
		enum {W_ID, W_INT, W_STR, W_LST};
		map<string, Data>::iterator it;
		int k, status = W_ID;
		Data* dest = NULL;
		bool error = false;

		for (k = begin; k < argc; k++) {
			if (status == W_ID) {
				if (isValidId(argv[k])) {
					it = variables.find(argv[k] + 1);
					if (it == variables.end()) {
						cout << "No existe el id " << argv[k] << endl;
						error = true;
					}
					else {
						dest = &(it->second);

						dest->timesSeen += 1;
						if (dest->timesSeen > 1) {
							cout << "Ya se había definido " << argv[k] << endl;
							error = true;
						}

						if (dest->type == T_BOOL) {
							dest->v_bool = true;
							dest->isSet = true;
						}
						else if (dest->type == T_INT) status = W_INT;
						else if (dest->type == T_STRING) status = W_STR;
						else if (dest->type == T_LIST) status = W_LST;
					}
				}
				else {
					cout << "Se esperaba un id en vez de " << argv[k] << endl;
					error = true;
				}
			}
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
						dest->isSet = true;
					}
					else {
						dest->v_list->push_back(argv[k]);
					}
				}
			}
		} // Fin for

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

		//if (error) return false;

		for (it = variables.begin(); it != variables.end(); it++) {
			if (it->second.isMandatory && it->second.timesSeen == 0) {
				cout << "El id -" << (it->first) << " es obligatorio" << endl;
				error = true;
			}
		}

		return !error;
	} // Fin parseArgs()

	///////////////
	//  SETTERS  //
	///////////////
	Config::Status Config::setValue(string name, bool value) {
		map<string, Data>::iterator it;
		cout << " llamado a set value bool" << endl;
		it = variables.find(name);
		if (it == variables.end()) return NOTFOUND;
		if (it->second.type != T_BOOL) return BADTYPE;
		it->second.v_bool = value;
		return SUCCESS;
	}

	Config::Status Config::setValue(string name, int value) {
		map<string, Data>::iterator it;
		cout << " llamado a set value int" << endl;
		it = variables.find(name);
		if (it == variables.end()) return NOTFOUND;
		if (it->second.type != T_INT) return BADTYPE;
		it->second.v_int = value;
		return SUCCESS;
	}

	Config::Status Config::setValue(string name, const char* value) {
		cout << " llamado a set value char*" << endl;
		return setValue(name, string(value));
	}

	Config::Status Config::setValue(string name, string value) {
		map<string, Data>::iterator it;
		cout << " llamado a set value string" << endl;
		it = variables.find(name);
		if (it == variables.end()) return NOTFOUND;
		if (it->second.type != T_STRING) return BADTYPE;
		cout << "a punto de borrar anterior" << endl;
		delete it->second.v_string;
		cout << "borrardo anterior" << endl;
		cout << "a punto de poner nueva" << endl;
		it->second.v_string = new string(value);
		cout << "puesta nueva" << endl;
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
}
