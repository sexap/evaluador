#include "validation.h"

namespace seap_implement {

	bool isDir(const string& path) {
		struct stat datos;
		if (stat(path.c_str(), &datos) != 0) return false;
		return S_ISDIR(datos.st_mode);
	}

	bool isFile(const string& path) {
		struct stat datos;
		if (stat(path.c_str(), &datos) != 0) return false;
		return S_ISREG(datos.st_mode);
	}

	bool isFileSmaller(const string& path, int maxSize) {
		struct stat datos;
		if (stat(path.c_str(), &datos) != 0) return false;
		if (!S_ISREG(datos.st_mode)) return false;
		return datos.st_size <= maxSize;
	}

	bool isValidJudgeType(const string& s) {
		return (s == "standard" || s == "special" || s == "interactive");
	}

	bool judgeNeedsExe(const string& s) {
		return (s == "special" || s == "interactive");
	}

	bool judgeNeedsOutput(const string& s) {
		return (s == "standard");
	}

	bool isValidAction(const string& s) {
		return (s == "eval");
	}

	bool isValidLang(const string& s) {
		return (s == "c" || s == "c++" || s == "java");
	}

	bool forceValidLang(string &s, const string& filename) {
		size_t p;
		string ext;

		p = filename.rfind('.');
		if (p == string::npos) return false;

		ext = filename.substr(p);
		if (ext == ".c") {
			s = "c";
			return true;
		}
		if (ext == ".cpp" || ext == ".cxx" || ext == ".c++") {
			s = "c++";
			return true;
		}
		if (ext == ".java") {
			s = "java";
			return true;
		}

		return false;
	}

	bool hasExtension(const string& s, const string& ext) {
		size_t p;
		string realExt;

		p = s.rfind('.');
		if (p == string::npos) return false;

		realExt = s.substr(p);
		return (realExt == "." + ext);
	}

	bool isBetween(int n, int min, int max) {
		return (n >= min && n <= max);
	}

	void forceBetween(int& n, int min, int max) {
		if (n < min) n = min;
		else if (n > max) n = max;
	}

}
