#ifndef GENERATOR_H
#define GENERATOR_H

#include <string>
#include <fstream>
#include <iostream>

#include "utils/validation.h"
#include "utils/string_manip.h"
#include "utils/process_control.h"

using namespace std;
using namespace seap_implement;

bool generator(const string& generator, const string& tempCase);

#endif // GENERATOR_H
