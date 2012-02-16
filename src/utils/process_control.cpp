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

}
