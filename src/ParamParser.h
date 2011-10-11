#ifndef PARAM_PASER_H
#define PARAM_PASER_H

#include <set>
#include <map>
#include <string>
#include <iostream>
using namespace std;

#include <cstring>
#include <cstdlib>

class ParamParser
{
	public:
		ParamParser();
		bool parse(int argc, char* argv[]);

		enum judgeType {normal,special, interactive};
	protected:

	private:
		//!
		/*! Indica si un archivo se puede abrir o no.
			\param name El nombre del archivo a verificar.
			\return true si el archivo existe y se puede abrir.
		*/
		bool isValidFile(const char* str);
		bool isValidLang(const char* str);
		bool isValidNumber(const char* str);
		bool isValidJudge(const char* str);

		string sourceFile;
		string language;
		string problem;
		judgeType judge;
		int maxCompileTime; // En milisegundos
		int maxCompileMem; // En kibi bytes
		int maxSourceFileSize; // En Kibi bytes
		bool verbose;
		bool strictEval;
		bool strictWhitespace;
		set<string> testCaseFiles;
		map<string, int> paramInfo;
};

#endif // PARAM_PASER_H

