#include <iostream>
using namespace std;

int vecinos[8][2] = {{+2,+1}, {+1,+2}, {-1,+2}, {-2,+1}, {-2,-1}, {-1,-2}, {+1, -2}, {+2, -1}};
int tablero[20][20] = {};
int best[20][20];
int posx, posy, n;
int mov = 1, movmin = 1000;

void rec() {
    int visit = 0;

    if (mov >= movmin) {
        return;
    }

    for (int i = 0; i < 8; ++i) {
        posx += vecinos[i][0];
        posy += vecinos[i][1];
        if (posx >= 0 && posx < n && posy >= 0 && posy < n && tablero[posx][posy] == 0) {
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

    if (visit == 0 && mov < movmin) {
        movmin = mov;
        for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) best[i][j] = tablero[i][j];
    }
}

int main() {
    cin >> n >> posx >> posy;

    --posx; --posy;
    tablero[posx][posy] = mov;
    rec();
    cerr << movmin << endl;
    for(int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) cout << best[i][j] << " ";
        cout << endl;
    }
    return 0;
}
