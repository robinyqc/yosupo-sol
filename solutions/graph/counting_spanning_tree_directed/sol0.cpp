#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <atcoder/modint>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;
using mint = atcoder::modint998244353;

mint determinant(vec<vec<mint>> a)
{
    u32 n = a.size(), w = 0;
    mint res = mint::raw(1);
    for (u32 i = 0; i < n; i++) {
        for (u32 j = i + 1; j < n; j++) {
            while (a[j][i] != 0) {
                if (a[i][i] != 0) {
                    mint quot = mint::raw(a[j][i].val() / a[i][i].val());
                    for (u32 k = i; k < n; k++) {
                        a[j][k] -= a[i][k] * quot;
                    }
                }
                w ^= 1u;
                a[i].swap(a[j]);
            }
        }
        res *= a[i][i];
    }
    if (w) return -res;
    return res;
}

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, m, r;
    cin >> n >> m >> r;
    vec<vec<mint>> a(n, vec<mint>(n));
    for (u32 i = 0, u, v; i < m; i++) {
        cin >> u >> v;
        ++a[v][v], --a[u][v];
    }
    a.erase(a.begin() + r);
    for (auto &i: a) i.erase(i.begin() + r);
    cout << determinant(std::move(a)).val() << endl;
    return 0;
}