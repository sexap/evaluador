#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

int main() {
	int tams[12] = {3,  5,  10,  20,   40,  80,  200,  500,  1000, 10000, 100000, 1000000};
	ofstream archivo;
	string name = "caso";
	for (int i = 0; i < 12; i++) {
		archivo.open((name + char('A'+i) + ".ent").c_str());
		archivo << tams[i] << endl;
		for (int k = 0; k < tams[i]; k++) {
			if (k > 0) archivo << " ";
			archivo << (rand() % (tams[i]*10))+1;
		}
		archivo << endl;
		archivo.close();
		system(("codes/invers00 > " + name + char('A'+i) + ".dat < " + name + char('A'+i) + ".ent").c_str());
	}
	return 0;
}
