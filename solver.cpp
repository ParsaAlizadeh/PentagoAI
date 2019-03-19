#include <iostream>
#include <map>
using namespace std;

#define X first
#define Y second

const int INF = 1e4;
char board[6][6];
int turn = 1;
int num = 0;

void draw()
{
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++)
            cout << board[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}

string rot90(char in[3][3] ,int k)
{
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

void rotPlate(int plate , int k)
{
    /* 0 2
       1 3 */
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

int inRow(int x, int y, int dx , int dy, int n = 5)
{
    int nx = x , ny = y;
    for(int i = 0 ; i < n ; i++) {
        if (!(0 <= nx && nx < 6 && 0 <= ny && ny < 6))  return 0;
        if(board[nx][ny] != board[x][y])    return 0;
        if(i == n-1)  return 1;
        nx += dx;
        ny += dy;
    }
}

pair<int , int> winScore()
{
    map<char, int> mp;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (inRow(i , j , 1 , 1) && board[i][j] != '_') {
                mp[board[i][j]] = 100;
            }
            if (inRow(5-i, j, -1, 1) && board[5-i][j] != '_') {
                mp[board[5-i][j]] = 100;
            }
        }
    }
    for(int i = 0 ; i < 6 ; i++) {
        for(int j = 0 ; j < 2 ; j++) {
            if(inRow(i , j , 0 , 1) && board[i][j] != '_') {
                mp[board[i][j]] = 100;
            }
            if(inRow(j , i , 1 , 0) && board[j][i] != '_') {
                mp[board[j][i]] = 100;
            }
       }
    }
    return {mp['X'], mp['O']};
}

bool endGame()
{
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 6; j++)
            if (board[i][j] == '_')
                return false;
    return true;
}

int eval()
{
    map<char, int> mp;
    for(int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            int x = i * 3, y = j * 3;
            if (inRow(x, y, 1, 1, 3) && board[x][y] != '_') {
                mp[board[x][y]]++;
            }
            if (inRow(x+2, y, -1, 1, 3) && board[x+2][y] != '_') {
                mp[board[x+2][y]]++;
            }

            for (int k = 0; k < 3; k++) {
                if (inRow(x+k, y, 0, 1, 3) && board[x+k][y] != '_') {
                    mp[board[x+k][y]]++;
                }
                if (inRow(x, y+k, 0, 1, 3) && board[x][y+k] != '_') {
                    mp[board[x+k][y]]++;
                }
            }
        }
    }
    double mX = mp['X'], mO = mp['O'];
    return mX * 50.0 / max(1.0, mX + mO);
}

int negamax(int col, int d, int alpha, int beta, int maxd)
{
    pair<int, int> score = winScore();
    if (endGame() || score.X > 0 || score.Y > 0) {
        return col * (score.X - score.Y);
    }
    if (d >= maxd) {
        return col * eval();
    }
    char mark = (col == 1? 'X' : 'O');
    int mx = -INF, val;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (board[i][j] == '_') {
                for (int k = 0; k < 4; k++) {
                    for (int l = -1; l < 2; l += 2) {
                        board[i][j] = mark;
                        rotPlate(k, l);
                        val = -negamax(-col, d + 1, -beta, -alpha, maxd);
                        rotPlate(k, -l);
                        board[i][j] = '_';
                        mx = max(mx, val);
                        alpha = max(alpha, mx);
                        if (beta <= alpha)  return mx;
                    }
                }
            }
        }
    }
    return mx;
}

pair<int, pair<int,int>> bestMove()
{
    int maxd = 0, ord = 1, q = 36 - num;
    while (ord < 1e7) {
        ord *= q * 8;
        maxd++;
        q--;
    }
    //cout << "maxd is " << maxd << endl;
    int mx = -INF, val;
    int maxMove, maxRot, maxClock;
    int alpha = -INF, beta = INF;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (board[i][j] == '_') {
                for (int k = 0; k < 4; k++) {
                    for (int l = -1; l < 2; l += 2) {
                        board[i][j] = 'X';
                        rotPlate(k, l);
                        val = -negamax(-1, 1, -beta, -alpha, maxd);
                        rotPlate(k, -l);
                        board[i][j] = '_';
                        if (val > mx) {
                            mx = val;
                            maxMove = i*6 + j;
                            maxRot = k;
                            maxClock = l;
                        }
                        alpha = max(alpha, mx);
                    }
                }
            }
        }
    }
    return {maxMove, {maxRot, maxClock}};
}

int main()
{
    freopen("in.txt" , "r" , stdin);
    freopen("out.txt" , "w" , stdout);

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            cin >> board[i][j];
        }
    }

    pair<int, int> score = {0, 0};
    int x, y, pl, k;

    cin >> x >> y >> pl >> k;
    if (x >= 0 && y >= 0){
        board[x][y] = 'O';
        rotPlate(pl, k);
    }

    score = winScore();
    if (endGame() || score.X > 0 || score.Y > 0) {
        cout << score.X << " " << score.Y << endl;
        return 0;
    }

    pair<int, pair<int, int>> move = bestMove();
    x = move.X / 6; y = move.X % 6;
    pl = move.Y.X; k = move.Y.Y;
    board[x][y] = 'X';
    rotPlate(pl, k);

    score = winScore();
    if (endGame() || score.X > 0 || score.Y > 0) {
        cout << score.X << " " << score.Y << endl;
        return 0;
    }

    draw();

    return 0;
}
