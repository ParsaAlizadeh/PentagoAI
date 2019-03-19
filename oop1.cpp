#include <bits/stdc++.h>
using namespace std;

#define X first
#define Y second

class State {
public:
    char board[6][6];
    set<State*> adj;
    pair<int, int> score;
    bool isRemain;

    State();
    //~State();

    void Fill(char);
    void Update();
    void draw();
    void rotPlate(int, int);
    void findAdj(char);
    bool inRow(int, int, int, int, int);
    bool endGame();
    int eval();
    pair<int, int> winScore();
    State Copy();
    string rot90(char[3][3], int);
};

map<State, State*> allState;

State::State() {
    Fill('_');
    score = winScore();
    isRemain = !(score.X > 0 || score.Y > 0 || endGame());
}

bool operator< (State a, State b) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (a.board[i][j] == b.board[i][j]) continue;
            return a.board[i][j] < b.board[i][j];
        }
    }
    return false;
}

bool operator== (State a, State b) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (a.board[i][j] != b.board[i][j])
                return false;
        }
    }
    return true;
}

void State::Fill(char c) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            board[i][j] = c;
        }
    }
}

State State::Copy() {
    State res;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            res.board[i][j] = board[i][j];
        }
    }
    return res;
}

void State::Update() {
    score = winScore();
    isRemain = !(score.X > 0 || score.Y > 0 || endGame());
    adj.clear();
}

void State::draw() {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++)
            cout << board[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}

string State::rot90(char in[3][3] ,int k) {
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

void State::rotPlate(int plate , int k)
{
    /*  0  2
        1  3  */
    int x = plate % 2 * 3 , y = plate / 2 * 3;
    char tmp[3][3];

    for(int i = x ; i < x + 3 ; i++)
        for(int j = y ; j < y + 3 ; j++)
            tmp[i - x][j - y] = board[i][j];

    string s = rot90(tmp , k);
    for(int i = x ; i < x + 3 ; i++)
        for(int j = y ; j < y + 3 ; j++)
            board[i][j] = s[(i - x) * 3 + j - y];
}

bool State::inRow(int x, int y, int dx , int dy, int n = 5) {
    if (board[x][y] == '_')
        return false;
    int nx = x , ny = y;
    for(int i = 0 ; i < n ; i++) {
        if (!(0 <= nx && nx < 6 && 0 <= ny && ny < 6))
            return false;
        if(board[nx][ny] != board[x][y])
            return false;
        if(i == n-1)
            return true;
        nx += dx;
        ny += dy;
    }
    return false;
}

pair<int , int> State::winScore() {
    map<char, int> mp;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (inRow(i , j , 1 , 1)) {
                mp[board[i][j]] = 100;
            }
            if (inRow(5-i, j, -1, 1)) {
                mp[board[5-i][j]] = 100;
            }
        }
    }
    for(int i = 0 ; i < 6 ; i++) {
        for(int j = 0 ; j < 2 ; j++) {
            if(inRow(i , j , 0 , 1)) {
                mp[board[i][j]] = 100;
            }
            if(inRow(j , i , 1 , 0)) {
                mp[board[j][i]] = 100;
            }
       }
    }
    return {mp['X'], mp['O']};
}

bool State::endGame() {
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 6; j++)
            if (board[i][j] == '_')
                return false;
    return true;
}

int State::eval() {
    map<char, int> mp;
    for(int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            int x = i * 3, y = j * 3;
            if (inRow(x, y, 1, 1, 3))
                mp[board[x][y]]++;
            if (inRow(x+2, y, -1, 1, 3))
                mp[board[x+2][y]]++;

            for (int k = 0; k < 3; k++) {
                if (inRow(x+k, y, 0, 1, 3))
                    mp[board[x+k][y]]++;
                if (inRow(x, y+k, 0, 1, 3))
                    mp[board[x+k][y]]++;
            }
        }
    }
    double all = mp['X'] + mp['O'];
    return mp['X'] * 50.0 / all;
}

void State::findAdj(char c) {
    if (!isRemain || adj.size() != 0)   return;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (board[i][j] != '_') continue;
            for (int k = 0; k < 4; k++) {
                for (int l = -1; l <= 1; l+=2){
                    State res = Copy();
                    res.board[i][j] = c;
                    res.rotPlate(k, l);
                    res.Update();
                    if (!allState.count(res))
                        allState[res] = &res;

                    adj.insert(allState[res]);
                }
            }
        }
    }
}

int main()
{
    State a;
    a.findAdj('X');
    for (State* b : a.adj) {
        State c = *b;
        c.draw();
        //*b).findAdj('O');
    }
    cout << a.adj.size() << " " << allState.size() << endl;
    return 0;
}
