#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

u64 qpow(u64 x, u64 y)
{
    u64 res = 1;
    for (; y; y >>= 1) {
        if (y & 1) res = res * x % 998244353;
        x = x * x % 998244353;
    }
    return res;
}

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, m;
    cin >> n >> m;
    vec<u32> a(n);
    for (auto &i: a) cin >> i;
    u32 l, r, v;
    while (m--) {
        cin >> l >> r >> v;
        u32 res = 0;
        for (u32 i = l; i + 1 < r; i++) {
            res ^= qpow(a[i], a[i + 1]);
        }
        for (u32 i = l; i < r; i++) {
            a[i] = v;
        }
        cout << res << '\n';
    }
    return 0;
}