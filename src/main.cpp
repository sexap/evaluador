#include <iostream>
using namespace std;

#include "ParamParser.h"

int main(int argc, char* argv[])
{
	ParamParser pp;
	return !pp.parse(argc, argv);
    return 0;
}
