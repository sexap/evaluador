#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
using namespace std;

int main() {
	int nums[19] = {2, 2, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8};
	ofstream archivo;
	string name = "caso_p";
	int n, r, c, m;
	for (int i = 0; i < 19; i++) {
		archivo.open((name + char('A'+i) + ".ent").c_str());
		n = nums[i];
		m = rand()%(2*n - 2) + 2;
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
