#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <list>
#include <map>
using namespace std;

#include <libconfig.h++>

#include "config_exceptions.h"

namespace seap_implement{

	class Config
	{
		public:
			Config();
			~Config();

			enum Type {T_BOOL, T_INT, T_STRING, T_LIST};
			enum Source {S_ARG, S_FILE, S_BOTH};

			//registerArgVar
			//registerFileVar
			//registerVar
			void registerVariable(string name, Type type, bool mandatory, Source source);
			bool parseArgs(int argc, char* argv[], int begin = 1);
			bool parseFile(const char* filename);
			bool validate();

			void setValue(string name, bool value);
			void setValue(string name, int value);
			void setValue(string name, const char* value);
			void setValue(string name, string value);

			void getValue(string name, bool& var);
			void getValue(string name, int& var);
			void getValue(string name, string& var);

			bool hasMoreItems(string name);
			void getNextItem(string name, string& var);
		protected:

		private:
			struct Data {
				Source source;
				Type type;
				bool isSet;
				bool isMandatory;
				int lastSeenBy;
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
			int sourceId;
	};

}

#endif // CONFIG_H

