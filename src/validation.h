#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>
using namespace std;

namespace seap_implement {

	/*! Indica si \c s describe un lenguaje válido. Los lenguajes válidos son
		\c c \c c++ y  c\ java
		\param s La cadena a evaluar.
		\return \c true Si la cadena es un lenguaje válido.
	*/
	bool isValidLang(const string& s);
	/*! Intenta asignar a \c s un lenguaje válido tomando en cuenta el nombre
		de archivo del código.
		Las extensiones válidas son \c .c para el lenguaje C;
		 \c .cpp,  \c .cxx y  \c .c++ para el lenguaje C++;
		 y \c .java para el lenguaje Java.
		\param s La cadena destino.
		\param filename El nombre de archivo del código.
		\return \c true si se logra reconocer el lenguaje de filename.
		\return \c false en caso contrario.
	*/
	bool forceValidLang(string & s, const string& filename);

	/*! Indica si \c s describe un tipo de juez válido.
		Los tipos de jueces son \c standar, \c special e \c interactive.
		\param s La cadena a evaluar.
		\return \c true Si la cadena es un lenguaje válido.
	*/
	bool isValidJudgeType(const string& s);

	/*! Indica si el enetro \c n se encuentra en el rango [min,max].
		\param n El número a evaluar.
		\param min, max El rango en que debe estar \c n.
		\return \c true Si n está en el rango.
		\return \c false en caso contrario.
	*/
	bool isBetween(int n, int min, int max);
	/*! Asigna al entero \c n el valor más cercano en el rango [min,max] .
		\param n El número a evaluar.
		\param min, max El rango en que debe estar \c n.
	*/
	void forceBetween(int& n, int min, int max);


}

#endif // VALIDATION_H

