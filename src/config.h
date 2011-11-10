#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <list>
#include <map>
using namespace std;

namespace seap_implement{
	class Config
	{
		public:
			enum Status {SUCCESS, BADTYPE, NOTFOUND, NOTSET, ERROR};
			enum Type {T_BOOL, T_INT, T_STRING, T_LIST};

			void registerVariable(string name, Type type, bool mandatory = false);
			bool parseArgs(int argc, char* argv[], int begin = 1);

			Status setValue(string name, bool value);
			Status setValue(string name, int value);
			Status setValue(string name, const char* value);
			Status setValue(string name, string value);

			Status getValue(string name, bool& var);
			Status getValue(string name, int& var);
			Status getValue(string name, string& var);

			bool hasMoreItems(string name);
			Status getNextItem(string name, string& var);

			Config();
			~Config();
		protected:

		private:
			struct Data {
				Type type;
				bool isSet;
				bool isMandatory;
				int timesSeen;
				list<string>::iterator listIt;
				union {
					bool v_bool;
					int v_int;
					string* v_string;
					list<string>* v_list;
				};
			};

			// Validaciones
			bool isValidId(const char* s);
			bool isValidInt(const char* s);

			map<string,Data> variables;
	};

}

#endif // CONFIG_H

