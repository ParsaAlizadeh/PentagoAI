#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<ll, ll> pll;

#define X first
#define Y second
#define sep ' '

const ll EVALPOINT = 100,
         WINPOINT = 1000;

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
            cout << get(hsh , ind[i][j]) << sep;
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

pll calcEval(ll hsh, int x, int y, int dx, int dy, int n)
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
        p = calcEval(hsh, 0, i, 1, 0, 3);
        cnt[1] += p.X;
        cnt[2] += p.Y;

        p = calcEval(hsh, i, 0, 0, 1, 3);
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
        p = calcEval(hsh, i, 0, 0, 1, 5);
        score[1] |= min(1LL, p.X);
        score[2] |= min(1LL, p.Y);

        p = calcEval(hsh, 0, i, 1, 0, 5);
        score[1] |= min(1LL, p.X);
        score[2] |= min(1LL, p.Y);
    }
    int bx[3] = {0, 0, 1},
       by[3] = {0, 1, 0};
    for (int i = 0; i < 3; i++) {
        p = calcEval(hsh, bx[i], by[i], 1, 1, 5);
        score[1] |= min(1LL, p.X);
        score[2] |= min(1LL, p.Y);

        p = calcEval(hsh, 5-bx[i], by[i], -1, 1, 5);
        score[1] |= min(1LL, p.X);
        score[2] |= min(1LL, p.Y);
    }
    return {score[1]*WINPOINT, score[2]*WINPOINT};
}

int main()
{
    preprocess();


    /*
    for(ll i = 0 ; i < 6 ; i++)
    {
        for(ll j = 0 ; j < 6 ; j++)
        {
            cout << ind[i][j] << sep;
        }
        cout << endl;
    } */

    ll hsh = 0;
    draw(hsh);

    int t, p, k;
    while (true) {
        cin >> t >> p;
        if (t == 2) {
            cin >> k;
            hsh = rotPlate(hsh, p, k);
        }
        else {
            hsh = modify(hsh, p, 1);
        }
        draw(hsh);
    }

    return 0;
}
