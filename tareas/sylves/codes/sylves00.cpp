#include <iostream>
using namespace std;

int main() {
	int n, m;
	int prev = 2, act, i;
	cin >> n >> m;
	for (i=0; i<n; i++) {
		act = (prev * (prev-1) + 1)%m;
		prev = act;
	}
	cout << prev << endl;
	return 0;
}
