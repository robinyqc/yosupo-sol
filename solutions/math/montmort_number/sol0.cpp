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
using mint = atcoder::dynamic_modint<0>;

template<typename Mint>
struct MontmorNumber
{
    vec<Mint> f;
    MontmorNumber(u32 n): f(n + 1)
    {
        f[0] = 1, f[1] = 0;
        for (u32 i = 2; i <= n; i++) {
            f[i] = (f[i - 1] + f[i - 2]) * (i - 1);
        }
    }
    Mint operator()(u32 x) const { return f[x]; }
};

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, m;
    cin >> n >> m;
    mint::set_mod(m);
    MontmorNumber<mint> mn(n);
    for (u32 i = 1; i <= n; i++) cout << mn(i).val() << ' ';
    cout << endl;
    return 0;
}