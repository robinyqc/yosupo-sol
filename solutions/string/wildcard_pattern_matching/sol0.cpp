#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <atcoder/modint>
#include <atcoder/convolution>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

using mint = atcoder::modint998244353;
using atcoder::convolution;

template<u32 num>
vec<mint> lift(const vec<mint> &a)
{
    vec<mint> res(a.size(), mint::raw(1));
    for (u32 i = 0; i < a.size(); i++) {
        for (u32 j = 0; j < num; j++) {
            res[i] *= a[i];
        }
    }
    return res;
}

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);

    string s, t;
    cin >> s >> t;

    u32 n = s.size(), m = t.size();
    vec<mint> a(n), b(m);

    for (u32 i = 0; i < n; i++) {
        if (s[i] == '*') a[i] = 0;
        else a[i] = s[i];
    }

    for (u32 i = 0; i < m; i++) {
        if (t[i] == '*') b[m - i - 1] = 0;
        else b[m - i - 1] = t[i];
    }

    auto x(convolution(lift<3>(a), b));
    auto y(convolution(lift<2>(a), lift<2>(b)));
    auto z(convolution(a, lift<3>(b)));

    for (u32 i = m - 1; i < n; i++) {
        if (x[i] + y[i] - 2 * z[i] == 0) cout.put('1');
        else cout.put('0');
    }
    cout.put('\n');
    return 0;
}