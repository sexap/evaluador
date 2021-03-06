#ifndef VALIDATION_H
#define VALIDATION_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <string>
using namespace std;

//! \file
//! \namespace seap_implement
namespace seap_implement {
	/*! Dependiendo de la versión indica si \c path es
		- un archivo ordinario accesible para lectura
		- un directorio accesible para lectura y listado
		- un archivo ejecutable accesible para ejecución
		\param path Ruta al archivo o directorio.
		\return \c true si \c path corresponde al tipo de archivo y permisos esperados,
		\c false en caso contrario o si hay algun error al abrir el archivo.
	*/
	bool isDir(const string& path);
	bool isFile(const string& path);
	bool isExec(const string& path);
	/*! Indica si el archivo indicado por \c path es de menor o igual tamaño
		que maxSize en kB
		\param path Ruta al archivo.
		\return \c true si el archivo es de menor o igual tamaño que maxSize,
		\c false en caso contrario o si hay algun error al abrir el archivo.
	*/
	bool isFileSmaller(const string& path, int maxSize);

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
		\return \c true si se logra reconocer el lenguaje de filename,
		\c false en caso contrario.
	*/
	bool forceValidLang(string & s, const string& filename);

	/*! Indica si \c s describe un tipo de juez válido.
		Los tipos de jueces son: \c standard, \c special e \c interactive.
		\param s La cadena a evaluar.
		\return \c true Si la cadena es un juez válido, \c false en caso contrario.
	*/
	bool isValidJudgeType(const string& s);

	/*! Indica si \c s describe un tipo de juez que requiera un ejecutable.
		Los tipos de jueces que requieren ejecutables son: \c special e \c interactive.
		\param s La cadena a evaluar.
		\return \c true Si la cadena es un juez que requiere ejecutable,
		\c false en caso contrario o si no es un juez válido.
	*/
	bool judgeNeedsExe(const string& s);

	/*! Indica si \c s describe un tipo de juez que requiera archivos de salida esperada.
		Los tipos de jueces que requieren salidas esperadas son: \c standard.
		\param s La cadena a evaluar.
		\return \c true Si la cadena es un juez que requiere archivos de salida esperada,
		\c false en caso contrario o si no es un juez válido.
	*/
	bool judgeNeedsOutput(const string& s);

	/*! Indica si \c s describe una acción válida para seap.
		Las acciones son: \c eval.
		\return \c true Si la cadena es una acción válida, \c false en caso contrario.
	*/
	bool isValidAction(const string& s);

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

