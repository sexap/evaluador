#ifndef JUEZ_ESPECIAL_H
#define JUEZ_ESPECIAL_H

#include <string>
#include <fstream>
#include <iostream>

#include "utils/validation.h"
#include "utils/string_manip.h"
#include "utils/process_control.h"

using namespace std;
using namespace seap_implement;

int juezEspecial(const string& archSalidaEsperada, const string& judgeExe);

#endif // JUEZ_ESPECIAL_H
