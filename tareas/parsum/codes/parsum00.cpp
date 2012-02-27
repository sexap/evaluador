#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
	//freopen("caso10.ent", "r", stdin);
	char c;
	int actual;
	int total = 0;
	while(cin >> c)
	{
		if(c == '(')
			total++;
	}
	cout << total << endl;
	return 0;
}
