#ifndef JUEZ_ESPECIAL_H
#define JUEZ_ESPECIAL_H

#include <string>
#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>

#include "utils/string_manip.h"

using namespace std;
using namespace seap_implement;

int juezEspecial(const string& archSalidaEsperada, const string& judgeExe);

#endif // JUEZ_ESPECIAL_H
