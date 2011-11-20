#include "validation.h"

namespace seap_implement {

	bool isValidJudgeType(const string& s) {
		return (s == "standard" || s == "special" || s == "interactive");
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

	bool isBetween(int n, int min, int max) {
		return (n >= min && n <= max);
	}

	void forceBetween(int& n, int min, int max) {
		if (n < min) n = min;
		else if (n > max) n = max;
	}

}
