/*
	IMPORTANTE: Utiliza la librería libconfig 1.4.8
	http://www.hyperrealm.com/libconfig/
	Enlazar con el archivo /usr/local/lib/libconfig++.a
*/


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
			enum Source {S_ARG, S_FILE, S_BOTH};

			//registerArgVar
			//registerFileVar
			//registerVar

			//! Registra una variable.
			/*! \param name El nombre de la variable.
				\param type El tipo de dato de la variable.
				\param mandatory Especifica si es obligatorio que el usuario defina la variable.
				\param source El origen de donde se leerá la variable.
			*/
			void registerVariable(string name, Type type, bool mandatory, Source source);
			//! Procesa los argumentos.
			/*! \param argc, argv Es la cantidad de argumentos pasados al programa
				y el vector de argumentos. Estos parámetros pueden ser pasados
				directamente de la función main del programa.
				\param begin Sólo este argumento y los siguientes serán analizados.
				\return \c true si no ocurrió ningún error.
			*/
			bool parseArgs(int argc, char* argv[], int begin = 1);
			//! Procesa un archivo.
			/*! \param filename La ruta y nmbre del archivo a analizar.
				\return \c true si no ocurrió ningún error.
			*/
			bool parseFile(const char* filename);
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
			void setValue(string name, bool value);
			void setValue(string name, int value);
			void setValue(string name, const char* value);
			void setValue(string name, string value);

			//! Obtiene el valor actual de la variable.
			/*! \param name El nombre de la variable.
				\param var La variable destino donde se almacenará una copia del valor actual.
				\throw ConfigExceptionNotFound Si la variable no ha sido registrada.
				\throw ConfigExceptionBadType Si la variable no es del mismo tipo
				que la variable destino.
				\throw ConfigExceptionNoData Si no se puede devolver ningún dato
				(únicamente en variables tipo cadena).
			*/
			void getValue(string name, bool& var);
			void getValue(string name, int& var);
			void getValue(string name, string& var);

			//! Indica si quedan más elementos en una lista
			/*! \param name El nombre de la variable.
				\return \c true Si quedan más elementos en la lista.
				\return \c false Si no.
				\throw ConfigExceptionNotFound Si la variable no ha sido registrada.
				\throw ConfigExceptionBadType Si la variable no es del tipo T_STRING.
			*/
			bool hasMoreItems(string name);
			//! Obtiene el valor del alemento actual de la lista.
			/*! \param name El nombre de la variable.
				\param var La variable destino donde se almacenará una copia del elemento actual.
				\throw ConfigExceptionNotFound Si la variable no ha sido registrada.
				\throw ConfigExceptionBadType Si la variable no es del tipo T_STRING.
				\throw ConfigExceptionNoData Si no ya no hay elementos en la lista.
			*/
			void getNextItem(string name, string& var);
		protected:

		private:
			//! Los datos necesarios para cada variable registrada
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

			//! Indica si s es un identificador válido
			bool isValidId(const char* s);
			//! Indica si s es un entero válido
			bool isValidInt(const char* s);

			map<string,Data> variables;
			int sourceId;
	};

}

#endif // CONFIG_H

