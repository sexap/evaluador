#ifndef PROCESS_CONTROL_H
#define PROCESS_CONTROL_H

#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
using namespace std;

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../build_params.h"

namespace seap_implement {

	struct resource_t {
		unsigned long long time;
		unsigned long long io_time;
		unsigned long mem;
	};

	void initResource(resource_t& res);

	void getMaxResourceUsage(pid_t pid, resource_t& res);

	//////////////////////////
	// Parametros para exec //
	//////////////////////////

	class ParamHolder
	{
		public:
			ParamHolder();
			~ParamHolder();

			void add(const char* s);
			void add(const std::string& s);
			void clear();

			char* const* params();
			const char*  exe();

			friend std::ostream& operator<< (std::ostream& os, const ParamHolder& ph);
		private:
			const char* paramList[MAX_EXEC_PARAMS];
			bool needsClean[MAX_EXEC_PARAMS];
			int pos;
	};

	std::ostream& operator<< (std::ostream& os, const ParamHolder& ph);

}

#endif // PROCESS_CONTROL_H

