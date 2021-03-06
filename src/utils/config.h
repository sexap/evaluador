/*
	IMPORTANTE: Utiliza la librería libconfig 1.4.8
	http://www.hyperrealm.com/libconfig/
	Enlazar con el archivo /usr/local/lib/libconfig++.a
*/


#ifndef CONFIG_H
#define CONFIG_H

#include <dirent.h> // opendir() getdir()
#include <stdio.h> // popen()
#include <string.h> // strlen()

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <list>
#include <map>
using namespace std;

#include <libconfig.h++>

#include "config_exceptions.h"
#include "../build_params.h"

namespace seap_implement{

	//! Lee variables de configuración.
	/*! Lee variables de configuración desde archivos de texto y desde los
		argumentos pasados al programa en la línea de comandos.

		Soporta cuatro \link Type tipos de datos \endlink que son:
		booleanos, enteros, cadenas y listas.

		Para leer una variable primero se debe registrar y cuando se procesen
		los argumentos o algún archivo la variable será buscada y
		reconocida automáticamente.

		Los archivos de configuración tienen el siguiente formato
		\code
		nombre_variable_1 = valor_1
		nombre_variable_2 = valor_2
		\endcode

		Los argumentos deben pasarse en el siguiente formato
		\code
		ejecutable -nombre_variable_1 valor_1 -nombre_variable_2 valor_2
		\endcode
	*/
	class Config
	{
		public:
			Config();
			~Config();

			//! Tipo de dato de las variables.
			enum Type {
				//! Tipo de dato boleano.
				T_BOOL,
				//! Tipo de dato número entero.
				T_INT,
				//! Tipo de dato cadena de caracteres.
				T_STRING,
				//! Tipo de dato lista de cadenas de caracteres.
				T_LIST
			};

			//! Registra una variable.
			/*! \param name El nombre de la variable.
				\param type El tipo de dato de la variable.
				\param position Si está presente indica una posición fija en los
					argumentos del programa donde se espera esta variable.
				\param mandatory Especifica si es obligatorio que el usuario defina la variable.
			*/
			void registerArgVar(const string& name, Type type, bool mandatory = false);
			void registerArgFixVar(const string& name, Type type);
			void registerFileVar(const string& name, Type type, bool mandatory = false);
			void registerVar(const string& name, Type type, bool mandatory = false);
			//! Procesa los argumentos.
			/*! \param argc, argv Es la cantidad de argumentos pasados al programa
				y el vector de argumentos. Estos parámetros pueden ser pasados
				directamente de la función main del programa.
				\param begin Sólo este argumento y los siguientes serán analizados.
				\return \c true si no ocurrió ningún error.
			*/
			bool parseArgs(int argc, char* argv[]);
			//! Procesa un archivo.
			/*! \param filename La ruta y nmbre del archivo a analizar.
				\return \c true si no ocurrió ningún error.
			*/
			bool parseFile(const string& filename);
			//! Valida las variables
			/*! Valida que todas las variables necesarias se hayan leido.
				\return \c true si todas las variables obligatorias se leyeron correctamente.
			*/
			bool validate();

			//! Asigna a una variable un valor.
			/*! \param name El nombre de la variable.
				\param value El nuevo valor de la variable.
				\throw ConfigExceptionNotFound Si la variable no ha sido registrada.
				\throw ConfigExceptionBadType Si la variable no es del mismo tipo
				que el nuevo valor que se trata de asignar.
			*/
			void setValue(const string& name, bool value);
			void setValue(const string& name, int value);
			void setValue(const string& name, const char* value);
			void setValue(const string& name, const string& value);

			//! Obtiene el valor actual de la variable.
			/*! \param name El nombre de la variable.
				\param var La variable destino donde se almacenará una copia del valor actual.
				\throw ConfigExceptionNotFound Si la variable no ha sido registrada.
				\throw ConfigExceptionBadType Si la variable no es del mismo tipo
				que la variable destino.
				\throw ConfigExceptionNoData Si no se puede devolver ningún dato
				(únicamente en variables tipo cadena).
			*/
			void getValue(const string& name, bool& var);
			void getValue(const string& name, int& var);
			void getValue(const string& name, unsigned& var);
			void getValue(const string& name, string& var);
			void getValue(const string& name, list<string>& var);

			//! Lista los archivos indicados en un archivo .set
			/*! Lista todos los archivos indicados en el archivo
				\c path. Siempre ignora las lineas en blanco.
				\param path El archivo del cual se listarán sus contenidos.
				\return La lista con los nombres de archivo.
			*/
			static list<string> getExpansion(const string& expr);

			//! Lista los archivos contenidos en una carpeta
			/*! Lista todos los archivos contenidos en la carpeta
				\c path. Si se especifica \c ext sólo se listan los archivos que
				terminen con esa extensión, si no se especifica se listan todos.
				Siempre ignora los archivos ocultos y subdirectorios de \c path.
				\param path El directorio del cual se listarán sus archivos.
				\param ext La extensión deseada de los archivos.
				\return La lista con los nombres de archivo.
			*/
			static list<string> getDirFiles(const string& path, const string& ext = "");
		protected:

		private:
			//!Origenes de las variables
			enum Source {S_ARG, S_FILE, S_BOTH};

			//! Los datos necesarios para cada variable registrada
			struct Data {
				Source source;
				Type type;
				bool isSet;
				bool isMandatory;
				int lastSeenBy;
				bool fixedPos;
				list<string>::iterator listIt;
				union {
					bool v_bool;
					int v_int;
					string* v_string;
					list<string>* v_list;
				};
			};

			//! Registra de verdad las variables
			void registerVariable(const string& name, Type type, int position, bool mandatory, Source source);

			//! Indica si s es un identificador válido
			bool isValidId(const char* s);
			//! Indica si s es un entero válido
			bool isValidInt(const char* s);

			map<string,Data> variables;
			list<Data*> fixedVariables;
			int sourceId;
			bool hasFailed;
	};

}

#endif // CONFIG_H

