#include "process_control.h"

namespace seap_implement {

	void initResource(resource_t& res) {
		res.time = 0;
		res.mem = 0;
	}

	void getMaxResourceUsage (pid_t pid, resource_t& res) {
		char path[128];
		unsigned long utime, stime, vsize;
		FILE* fd;

		sprintf(path, "/proc/%d/stat", pid);
		fd = fopen(path, "r");
		if (fd == NULL) {
			printf ("ERRORROORORORO\n");
		}
		else {
			// Leer ignorando casi todo, exepto memoria y tiempo
			// (http://www.kernel.org/doc/man-pages/online/pages/man5/proc.5.html)
			fscanf(fd, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %*d %*d %*d %*d %*d %*d %*u %lu", &utime, &stime, &vsize);
			// Asignar el tiempo
			res.time = utime + stime;
			// Asignar la memoria máxima
			if (vsize > res.mem) res.mem = vsize;
			fclose(fd);
		}
	}

	//////////////////////////
	// Parametros para exec //
	//////////////////////////

	ParamHolder::ParamHolder() {
		paramList[0] = NULL;
		pos = 0;
	}

	void ParamHolder::add(const char* s) {
		if (pos+1 < MAX_EXEC_PARAMS) {
			paramList[pos] = s;
			paramList[pos+1] = NULL;
			needsClean[pos] = false;
			++pos;
		}
	}

	void ParamHolder::add(const std::string& s) {
		char* aux;

		if (pos+1 < MAX_EXEC_PARAMS) {
			aux = new char[s.length()+1];
			strcpy(aux, s.c_str());

			paramList[pos] = aux;
			paramList[pos+1] = NULL;
			needsClean[pos] = true;
			++pos;
		}
	}

	char* const* ParamHolder::params() {
		return (char* const*) paramList;
	}

	const char* ParamHolder::exe() {
		return paramList[0];
	}

	void ParamHolder::clear() {
		for (int i = 0; i < pos; ++i) {
			if (needsClean[i]) delete paramList[i];
		}
		paramList[0] = NULL;
		pos = 0;
	}

	ParamHolder::~ParamHolder() {
		clear();
	}

	std::ostream& operator<< (std::ostream& os, const ParamHolder& ph) {
		for (int i = 0; i < ph.pos; ++i) {
			os << (i>0?" ":"") << ph.paramList[i];
		}
		return os;
	}

}
