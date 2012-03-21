#include <iostream>
using namespace std;

int vecinos[8][2] = {{+2,+1}, {+1,+2}, {-1,+2}, {-2,+1}, {-2,-1}, {-1,-2}, {+1, -2}, {+2, -1}};
int tablero[20][20] = {};
int best[20][20];
int posx, posy, n, m;
int mov = 1, movmax = 0;

bool bestfound = false;
int cota;

unsigned long long int caminos = 0;

void rec() {
    int visit = 0;

    if (bestfound) return;

    for (int i = 0; i < 8; ++i) {
        posx += vecinos[i][0];
        posy += vecinos[i][1];
        if (posx >= 0 && posx < n && posy >= 0 && posy < n && (posx+posy+2)%m != 0 && tablero[posx][posy] == 0) {
            ++visit;
            ++mov;
            tablero[posx][posy] = mov;
            rec();
            tablero[posx][posy] = 0;
            --mov;
        }
        posx -= vecinos[i][0];
        posy -= vecinos[i][1];
    }

	if (visit == 0) ++caminos;

    if (visit == 0 && mov > movmax) {
        movmax = mov;
        for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) best[i][j] = tablero[i][j];
    }

    if (mov == cota) {
		//cerr << "no hay mejor" << endl;
		bestfound = true;
	}
}

int main() {
    cin >> n >> posx >> posy >> m;

    cota = n*n;
    for(int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) if ((i+j+2)%m == 0) --cota;

    --posx; --posy;
    tablero[posx][posy] = mov;
    rec();

	//cerr << "caminos: " << caminos << endl;
	cerr << movmax << endl;
    for(int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
		cout << best[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}
