#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

int main() {
	int nums[12] = {1,  2,  3,  5,   8,  10,  20,  30,  50, 100, 500, 1000};
	int mods[12] = {5, 11, 19, 59, 157, 331, 383, 503, 641, 769, 977,  997};
	ofstream archivo;
	string name = "caso";
	for (int i = 0; i < 12; i++) {
		archivo.open((name + char('A'+i) + ".ent").c_str());
		archivo << nums[i] << " " << mods[i] << endl;
		archivo.close();
		system(("codes/sylves00 > " + name + char('A'+i) + ".dat < " + name + char('A'+i) + ".ent").c_str());
	}
	return 0;
}
