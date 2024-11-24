// Pentagonal Number Theorem + Inv of Formal Power Series (Sparse)
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <atcoder/convolution>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

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

template<typename Mint>
vec<Mint> partition_function_table(u32 n)
{
    vec<pair<u32, Mint>> f;
    f.emplace_back(0, 1);
    for (u32 i = 1; i * (3 * i - 1) <= n * 2; i++) {
        Mint v = (i & 1 ? Mint::raw(Mint::mod() - 1) : Mint::raw(1));
        f.emplace_back(i * (3 * i - 1) / 2, v);
        f.emplace_back(i * (3 * i + 1) / 2, v);
    }
    return inv_poly_sparse(f, n + 1);
}

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n;
    cin >> n;
    using mint = atcoder::modint998244353;
    auto pf(partition_function_table<mint>(n));
    for (auto i: pf) cout << i.val() << ' ';
    cout << endl;
    return 0;
}