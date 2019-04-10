#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<ll, ll> pll;

#define X first
#define Y second
#define sep ' '

const ll EVALPOINT = 100;

ll ind[6][6];
ll hsh;
ll pw[40];
ll rot[ll(2e4)][2];

ll get(ll hsh , ll l , ll r)
{
    return hsh % pw[r + 1] / pw[l];
}

ll get(ll hsh , ll x)
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

ll modify(ll hsh , ll x , ll val)
{
    return modify(hsh , x , x , val);
}

ll draw(ll hsh)
{
    for(ll i = 0 ; i < 6 ; i++ , cout << endl)
    {
        for(ll j = 0 ; j < 6 ; j++)
        {
            cout << get(hsh , ind[i][j]) << sep;
        }
    }
    cout << endl;
}

ll rot90(int in[3][3] ,int k)
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

ll preprocess()
{
    for (ll i = 0; i < pw[9]; i++) {
        int tmp[3][3];
        for (ll j = 0; j < 3; j++) {
            for (ll k = 0; k < 3; k++) {
                tmp[j][k] = get(i, ind[j][k]);
            }
        }
        rot[i][0] = rot90(tmp, -1);
        rot[i][1] = rot90(tmp, +1);
    }
}

pll calcEval(ll hsh , ll x , ll y , ll dx , ll dy)
{
    ll cnt[3] = {0 , 0 , 0};
    ll t = 1 , cur = get(hsh , ind[x][y]);
    while(0 <= x && x < 6 && 0 <= y && y < 6)
    {
        if(get(hsh , ind[i][j]) == cur) t++;
        else
        {
            cnt[cur] += max(t - 2 , 0LL);
            t = 1;
            cur = get(hsh , ind[i][j]);
        }
        x += dx;
        y += dy;
    }
    cnt[cur] += max(t - 2 , 0LL);
    return {cnt[1] , cnt[2]};
}

ll eval(ll hsh)
{
    ll cnt[3] = {0 , 0 , 0};
    for(ll i = 0 ; i < 6 ; i++)
    {
        pll p = calcEval(hsh , 0 , i , 1 , 0);
        cnt[1] += p.X;
        cnt[2] += p.Y;
        p = calcEval(hsh , i , 0 , 0 , 1);
        cnt[1] += p.X;
        cnt[2] += p.Y;
    }

    double all = cnt[1] + cnt[2];
    return (cnt[1] * EVALPOINT / all) - (cnt[2] * EVALPOINT / all);
}

int main()
{
    pw[0] = 1;
    for(ll i = 1 ; i < 40 ; i++)    pw[i] = pw[i - 1] * 3;

    ll t = 0;
    for(ll i = 0 ; i < 4 ; i++)
    {
        ll lx = i % 2 * 3 , ly = i / 2 * 3;
        for(ll j = 0 ; j < 3 ; j++)
        {
            for(ll k = 0 ; k < 3 ; k++)
            {
                ind[lx + j][ly + k] = t++;
            }
        }
    }

    /*
    for(ll i = 0 ; i < 6 ; i++)
    {
        for(ll j = 0 ; j < 6 ; j++)
        {
            cout << ind[i][j] << sep;
        }
        cout << endl;
    } */

    /*
    for(ll i = 0 ; i < 6 ; i++)
    {
        for(ll j = 0 ; j < 6 ; j++)
        {
            char x;
            cin >> x;
            hsh += getValue(x) * pw[ind[i][j]];
        }
    } */

    preprocess();
    ll hsh = 0;
    draw(hsh);
    hsh = modify(hsh, 0, 2, 13);
    hsh = modify(hsh, 3, 5, pw[3]-1);
    draw(hsh);

    ll p, k;
    while (true) {
        cin >> p >> k;
        hsh = rotPlate(hsh, p, k);
        draw(hsh);
        cout << eval(hsh) << endl;
    }

    return 0;
}
