#ifndef PROCESS_CONTROL_H
#define PROCESS_CONTROL_H

#include <stdio.h>
#include <unistd.h>

namespace seap_implement {

	struct resource_t {
		unsigned long long time;
		unsigned long mem;
	};

	void initResource(resource_t& res);

	void getMaxResourceUsage(pid_t pid, resource_t& res);

}

#endif // PROCESS_CONTROL_H

