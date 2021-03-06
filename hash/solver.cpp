#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<ll, ll> pll;

#define X first
#define Y second
#define sep ' '

const ll EVALPOINT = 100,
         WINPOINT = 1000,
         INF = 1e9;

ll ind[6][6],
   pw[40],
   rot[ll(2e4)][2];

ll hsh;

void fillPower()
{
    pw[0] = 1;
    for (int i = 1; i < 40; i++) {
        pw[i] = pw[i-1] * 3;
    }
}

void fillInd()
{
    int t = 0;
    for(int i = 0 ; i < 4 ; i++) {
        int lx = i % 2 * 3 , ly = i / 2 * 3;
        for(int j = 0 ; j < 3 ; j++) {
            for(int k = 0 ; k < 3 ; k++) {
                ind[lx + j][ly + k] = t++;
            }
        }
    }
}

ll get(ll hsh, ll l, ll r)
{
    return hsh % pw[r + 1] / pw[l];
}

ll get(ll hsh, ll x)
{
    return get(hsh, x, x);
}

ll getValue(char c)
{
    return (c == '_' ? 0 : (c == 'X' ? 1 : 2));
}

char getChar(int c)
{
    return (c == 0 ? '_' : (c == 1 ? 'X' : 'O'));
}

ll modify(ll hsh, ll l, ll r, ll val)
{
    return hsh % pw[l] + val * pw[l] + hsh / pw[r + 1] * pw[r + 1];
}

ll modify(ll hsh, ll x, ll val)
{
    return modify(hsh, x, x, val);
}

ll draw(ll hsh)
{
    for(int i = 0 ; i < 6 ; i++) {
        for(int j = 0 ; j < 6 ; j++) {
            cout << getChar(get(hsh , ind[i][j])) << sep;
        }
        cout << endl;
    }
    cout << endl;
}

ll rot90(int in[3][3], int k)
{
    int tmp[3][3];
    ll ans = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (k == 1)
                tmp[i][j] = in[2-j][i];
            else
                tmp[i][j] = in[j][2-i];
            ans = modify(ans, ind[i][j], tmp[i][j]);
        }
    }
    return ans;
}

ll rotPlate(ll hsh, ll ind, ll k)
{
    ll l = ind * 9, r = ind * 9 + 8;
    ll plate = get(hsh, l, r);
    plate = rot[plate][(k+1)/2];
    return modify(hsh, l, r, plate);
}

void preprocess()
{
    fillPower();
    fillInd();
    for (int i = 0; i < pw[9]; i++) {
        int tmp[3][3];
        for (int j = 0; j < 3; j++) {
            for (ll k = 0; k < 3; k++) {
                tmp[j][k] = get(i, ind[j][k]);
            }
        }
        rot[i][0] = rot90(tmp, -1);
        rot[i][1] = rot90(tmp, +1);
    }
}

pll inRow(ll hsh, int x, int y, int dx, int dy, int n)
{
    int cnt[3] = {0 , 0 , 0};
    int t = 1 , cur = get(hsh , ind[x][y]);
    x += dx; y += dy;
    while(0 <= x && x < 6 && 0 <= y && y < 6) {
        if(get(hsh , ind[x][y]) == cur)
            t++;
        else {
            cnt[cur] += max(t - n + 1 , 0);
            t = 1;
            cur = get(hsh , ind[x][y]);
        }
        x += dx;
        y += dy;
    }
    cnt[cur] += max(t - n + 1 , 0);
    return {cnt[1] , cnt[2]};
}

ll eval(ll hsh)
{
    int cnt[3] = {0, 0, 0};
    pll p;
    for(int i = 0 ; i < 6 ; i++) {
        p = inRow(hsh, 0, i, 1, 0, 3);
        cnt[1] += p.X;
        cnt[2] += p.Y;

        p = inRow(hsh, i, 0, 0, 1, 3);
        cnt[1] += p.X;
        cnt[2] += p.Y;
    }
    double all = cnt[1] + cnt[2];
    return (cnt[1] * EVALPOINT / all) - (cnt[2] * EVALPOINT / all);
}

pll winScore(ll hsh)
{
    int score[3] = {0, 0, 0};
    pll p;
    for (int i = 0; i < 6; i++) {
        p = inRow(hsh, i, 0, 0, 1, 5);
        score[1] |= min(1LL, p.X);
        score[2] |= min(1LL, p.Y);

        p = inRow(hsh, 0, i, 1, 0, 5);
        score[1] |= min(1LL, p.X);
        score[2] |= min(1LL, p.Y);
    }
    int bx[3] = {0, 0, 1},
       by[3] = {0, 1, 0};
    for (int i = 0; i < 3; i++) {
        p = inRow(hsh, bx[i], by[i], 1, 1, 5);
        score[1] |= min(1LL, p.X);
        score[2] |= min(1LL, p.Y);

        p = inRow(hsh, 5-bx[i], by[i], -1, 1, 5);
        score[1] |= min(1LL, p.X);
        score[2] |= min(1LL, p.Y);
    }
    return {score[1]*WINPOINT, score[2]*WINPOINT};
}

pll negamax(ll hsh, int col, int d, int alpha, int beta, int maxd)
{
    pll scores = winScore(hsh);
    if (d == 36 || scores.X > 0 || scores.Y) {
        return {col * (scores.X - scores.Y), hsh};
    }
    if (d >= maxd) {
        return {col * eval(hsh), hsh};
    }
    int mark = (col == 1 ? 1 : 2), val, mx = -INF;
    ll best, now;
    pll res;
    for (int i = 0; i < 36; i++) {
        if (get(hsh, i) == 0) {
            now = modify(hsh, i, mark);
            for (int j = 0; j < 4; j++) {
                for (int k = -1; k < 2; k += 2) {
                    now = rotPlate(val, j, k);
                    res = negamax(now, -col, d+1, -beta, -alpha, maxd);
                    val = -res.X;
                    if (val > mx) {
                        mx = val;
                        best = now;
                        alpha = max(alpha, mx);
                    }
                    if (beta <= alpha)  return {mx, best};
                }
            }
        }
    }
    return {mx, best};
}

int main()
{
    preprocess();

    ll hsh = 0;
    int d = 0;
    char c;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            cin >> c;
            hsh = modify(hsh, ind[i][j], getValue(c));
            d += (c != '_');
        }
    }
    draw(hsh);
    cout << d;

    /*pll scores = winScore(hsh);
    if (d == 36 || scores.X > 0 || scores.Y > 0) {
        cout << scores.X << sep << scores.Y << sep << (d == 36) << endl;
        draw(hsh);
    }

    int maxd = d;
    ll ord = 1;
    while (ord * (36 - maxd) * 8 <= 1e7) {
        ord *= (36 - maxd) * 8;
        maxd++;
    }

    ll next = negamax(hsh, 1, d, -INF, INF, maxd).Y;
    scores = winScore(next);
    cout << scores.X << sep << scores.Y << sep << (d == 36) << endl;
    draw(next);*/

    return 0;
}
