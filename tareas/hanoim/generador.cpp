#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

int main() {
	int tams[12] = {1, 2, 3, 4, 5, 7, 9, 11, 13, 15};
	ofstream archivo;
	string name = "caso";
	for (int i = 0; i < 10; i++) {
		archivo.open((name + char('A'+i) + ".ent").c_str());
		archivo << tams[i] << endl;
		archivo.close();
	}
	return 0;
}
