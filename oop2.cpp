#include "bits/stdc++.h"
using namespace std;

#define X first
#define Y second

class Board {
public:
    static map<Board*, vector<Board*>> adj;
    static map<Board, Board*> all;

    Board* par;
    char data[6][6];

    Board();
    Board(Board*);
    ~Board();

    void Fill(char);
    void draw();


    string rot90(char[3][3], int);
    void rotPlate(int, int);

};

bool operator<(Board &a, Board &b) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (a.data[i][j] == b.data[i][j])
                continue;
            return a.data[i][j] < b.data[i][j];
        }
    }
}

Board::Board() {
    par = nullptr;
    Fill('_');
}

Board::Board(Board* oth) {
    par = oth;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            this->data[i][j] = oth->data[i][j];
        }
    }
}

Board::~Board() {
    cout << "board dead" << endl;
}

void Board::Fill(char c) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            this->data[i][j] = c;
        }
    }
}

void Board::draw() {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++)
            cout << this->data[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}

string Board::rot90(char in[3][3] ,int k) {
    char tmp[3][3];
    string ans = "";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (k == 1)
                tmp[i][j] = in[2-j][i];
            else
                tmp[i][j] = in[j][2-i];
            ans += string(1 , tmp[i][j]);
        }
    }
    return ans;
}

void Board::rotPlate(int plate , int k) {
    /*  0  2
        1  3  */
    int x = plate % 2 * 3 , y = plate / 2 * 3;
    char tmp[3][3];

    for(int i = x ; i < x + 3 ; i++)
        for(int j = y ; j < y + 3 ; j++)
            tmp[i - x][j - y] = this->data[i][j];

    string s = rot90(tmp , k);
    for(int i = x ; i < x + 3 ; i++)
        for(int j = y ; j < y + 3 ; j++)
            this->data[i][j] = s[(i - x) * 3 + j - y];
}

int main()
{
    Board* a = new Board();
    Board* b = new Board(a);
    b->data[0][0] = 'X';
    a->draw();
    b->draw();
    b->rotPlate(0, 1);
    b->draw();
    delete a;
    delete b;
    return 0;
}
