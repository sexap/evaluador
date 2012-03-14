#ifndef JUEZ_INTERACTIVO_H
#define JUEZ_INTERACTIVO_H

#include <string>
#include <fstream>
#include <iostream>
using namespace std;

#include "utils/process_control.h"
using namespace seap_implement;

pid_t juezInteractivoStart(const string& caseFile, const string& judgeExe, int alumno_in, int alumno_out);
bool juezInteractivoEnd(pid_t pID);
int juezInteractivoResult();

#endif // JUEZ_INTERACTIVO_H

