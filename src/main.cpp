#include <iostream>
using namespace std;

#include "config.h"
using namespace seap_implement;

int main(int argc, char* argv[]) {
	seap_implement::Config cfg;
	bool var1;
	int var2;
	string var3;

	//cfg.registerVariable("b", Config::T_BOOL, true, Config::S_ARGV);
	cfg.registerVariable("n", Config::T_BOOL, true, Config::S_FILE);
	//cfg.registerVariable("s", Config::T_STRING, true, Config::S_BOTH);
	cfg.registerVariable("l", Config::T_LIST);

	cfg.parseFile("conf");
	cfg.parseArgs(argc, argv);
	cfg.validate();

	cfg.getValue("n", var1);
	cout << boolalpha << var1 << endl;

    return 0;
}
