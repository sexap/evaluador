#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

int main() {
	int nums[12] = {1,  2,  3,  5, 6, 8, 10, 12, 14, 16, 18, 20};
	ofstream archivo;
	string name = "caso";
	for (int i = 0; i < 12; i++) {
		archivo.open((name + char('A'+i) + ".ent").c_str());
		archivo << nums[i] << " " << (rand()%nums[i])+1 << " " << (rand()%nums[i])+1 << endl;
		archivo.close();
		system(("codes/flojo 2> " + name + char('A'+i) + ".dat < " + name + char('A'+i) + ".ent").c_str());
	}
	return 0;
}
