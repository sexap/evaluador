#include <iostream>
using namespace std;

#include "config.h"
using namespace seap_implement;

int main(int argc, char* argv[]) {
	Config cfg;
	bool var1;
	int var2;
	string var3;

	cfg.registerVariable("b", Config::T_BOOL);
	cfg.setValue("b", false);
	cfg.registerVariable("n", Config::T_INT);
	cfg.registerVariable("s", Config::T_STRING);
	cfg.setValue("s", string("default"));
	cfg.registerVariable("l", Config::T_LIST, true);
	if (cfg.parseArgs(argc, argv)) {
		cout << "Finalización normal" << endl;
		if (cfg.getValue("b", var1) == Config::SUCCESS) cout << "success" << endl;
		else cout << "not set" << endl;
		cout << boolalpha << var1 << endl;

		if (cfg.getValue("n", var2) == Config::SUCCESS) cout << "success" << endl;
		else cout << "not set" << endl;
		cout << var2 << endl;

		if (cfg.getValue("s", var3) == Config::SUCCESS) cout << "success" << endl;
		else cout << "not set" << endl;
		cout << var3 << endl;

		cout << "lista: " << endl;
		while(cfg.hasMoreItems("l")) {
			cfg.getNextItem("l", var3);
			cout << var3 << endl;
		}
	}
    return 0;
}
