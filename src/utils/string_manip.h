#ifndef STRING_MANIP_H
#define STRING_MANIP_H

#include <string>
using namespace std;

//! \file
//! \namespace seap_implement
namespace seap_implement {

	/*! Indica si la cadena \c s tiene la extensión \c ext indicada.
		\param s La cadena a verificar.
		\param ext La extensión deseada
		\return \c true si la cadena s termina en \c .ext, \c false en caso contrario.
	*/
	bool hasExtension(const string& s, const string& ext);

	/*! Devuelve la cadena \c s sin extensión. Si la cadena no tiene extensión se devuelve igual.
		\param s la cadena a modificar
		\return La cadena sin extensión.
	*/
	const string removeExtension(const string& s);

	/*! Devuelve sólo el nombre de archivo del path \c s.
		Por ejemplo:
			\code
			getFileName("/home/hola.txt") devuelve "hola.txt"
			getFileName("/home/directorio/") devuelve ""
			\endcode
		\param s la cadena a modificar
		\return La cadena sin extensión.
		\note La función no verifica si realmente es un directorio o no. Sólo manipulala cadena.
	*/
	const string getFileName(const string& s);

	/*! Devuelve la cadena \c s sin el '/' final. Si la cadena no termina con '/' se devuelve igual.
		\param s la cadena a modificar
		\return La cadena sin '/'.
	*/
	const string cleanDirName(const string& s);

}

#endif // STRING_MANIP_H

