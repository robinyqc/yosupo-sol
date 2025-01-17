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

template<typename Mint>
struct Binom_PMod
{
    vec<Mint> fac, inv_fac;

    Binom_PMod(u32 n): fac(n + 1), inv_fac(n + 1)
    {
        fac[0] = Mint(1);
        for (u32 i = 1; i <= n; i++) fac[i] = fac[i - 1] * i;
        inv_fac[n] = fac[n].inv();
        for (u32 i = n - 1; ~i; i--) inv_fac[i] = inv_fac[i + 1] * (i + 1);
    }

    Mint operator()(int n, int m)
    {
        if (m < 0 || m > n) return Mint(0);
        return fac[n] * inv_fac[m] * inv_fac[n - m];
    }
};

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    using mint = atcoder::dynamic_modint<0>;
    u32 t, m, n, k;
    cin >> t >> m;
    mint::set_mod(m);
    Binom_PMod<mint> binom(min(m, 10000000u) - 1);
    while (t--) {
        cin >> n >> k;
        cout << binom(n, k).val() << '\n';
    }
    return 0;
}