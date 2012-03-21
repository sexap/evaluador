#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
using namespace std;

int main() {
	int nums[27] = {7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 14, 15, 15, 15};
	ofstream archivo;
	string name = "caso_g";
	int n, r, c, m;
	for (int i = 0; i < 27; i++) {
		archivo.open((name + char('A'+i) + ".ent").c_str());
		n = nums[i];
		do {
			m = (rand() % int(sqrt(n))-1) + 2;
		} while (m < 2);
		do {
			r = rand()%n + 1;
			c = rand()%n + 1;
		} while ((r+c)%m == 0);
		archivo << n << " " << r << " " << c << " " << m << endl;
		archivo.close();
		
		archivo.open((name + char('A'+i) + ".dat").c_str());
		archivo << 0 << endl;
		archivo.close();
	}
	return 0;
}
