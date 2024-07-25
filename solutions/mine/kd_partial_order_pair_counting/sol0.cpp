#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <bitset>
#include <cmath>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

signed main() 
{
    u32 k, n;
    cin >> k >> n;
    vec<vec<pair<u32, u32>>> p(k, vec<pair<u32, u32>>(n));
    for (u32 i = 0; i < n; i++) {
        for (u32 d = 0; d < k; d++) {
            cin >> p[d][i].first;
            p[d][i].second = i;
        }
    }
    for (u32 d = 0; d < k; d++) sort(p[d].begin(), p[d].end());
    vec<u32> ans(n);
    const u32 bs = ceil(sqrt(n)) * 16;
    for (u32 i = 0; i * bs < n; i++) {
        u32 l = i * bs, r = min(n, i * bs + bs);
        vec<bitset<80000>> b(r - l);
        for (auto &c: b) c.set();
        for (u32 d = 0; d < k; d++) {
            bitset<80000> s;
            for (u32 j = 0, it = 0; j < n; j++) {
                auto [v, q] = p[d][j];
                while (it < n && p[d][it].first <= v) s.set(p[d][it++].second);
                if (l <= q && q < r) b[q - l] &= s;
            }
        }
        for (u32 j = l; j < r; j++) ans[j] = b[j - l].count();
    }
    for (u32 i = 0; i < n; i++) cout << ans[i] << '\n';
    return 0;
}