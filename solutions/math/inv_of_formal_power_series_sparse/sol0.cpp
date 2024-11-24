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

// Exists x that a[x].first == 0 and a[x].second != 0
template<typename Mint>
vec<Mint> inv_poly_sparse(const vec<pair<u32, Mint>> &a, u32 tlen)
{
    vec<Mint> f(tlen);
    Mint iv;
    for (auto [x, v]: a) {
        if (x == 0) {
            iv = v.inv();
            break;
        }
    }
    assert(iv.val() != 0);
    f[0] = iv;
    iv = -iv;
    for (u32 i = 1; i < tlen; i++) {
        Mint nw;
        for (auto [x, v]: a) {
            if (0 < x && x <= i) {
                nw += v * f[i - x];
            }
        }
        f[i] = nw * iv;
    }
    return f;
}

signed main() 
{
    using mint = atcoder::modint998244353;
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, k;
    cin >> n >> k;
    vec<pair<u32, mint>> a(k);
    for (auto &[x, v]: a) {
        static u32 y;
        cin >> x >> y;
        v = mint::raw(y);
    }
    auto f(inv_poly_sparse(a, n));
    for (auto i: f) cout << i.val() << ' ';
    cout << endl;
    return 0;
}