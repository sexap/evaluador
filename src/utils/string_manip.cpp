#include "string_manip.h"

namespace seap_implement {

	bool hasExtension(const string& s, const string& ext) {
		size_t p;
		string realExt;

		p = s.rfind('.');
		if (p == string::npos) return false;

		realExt = s.substr(p);
		return (realExt == "." + ext);
	}

	const string removeExtension(const string& s) {
		return s.substr(0, s.rfind('.'));
	}

	const string getFileName(const string& s) {
		size_t found = s.rfind('/');
		if (found != string::npos) return s.substr(found + 1);
		else return s;
	}

	const string cleanDirName(const string& s) {
		if (s.length() == 0) return s;
		if (s[s.length()-1] == '/') return s.substr(0, s.length()-1);
		return s;
	}

}
