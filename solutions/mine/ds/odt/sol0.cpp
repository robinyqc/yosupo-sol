#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <map>
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
    map<pair<u32, u32>, u32> seg;
    for (u32 i = 0, l = 0; i < n; i++) {
        if (i + 1 == n || a[i] != a[i + 1]) {
            seg.emplace_hint(seg.end(), pair{l, i + 1}, a[i]);
            l = i + 1;
        }
    }
    
    // 先分裂右端点再分裂左端点！
    auto split = [&](u32 x)
    {
        if (x == n) return seg.end();
        auto it = seg.lower_bound({x, 0});
        if (it != seg.end() && it->first.first == x) return it;
        auto [l, r] = (--it)->first;
        auto v = it->second;
        it = seg.erase(it);
        seg.emplace_hint(it, pair{l, x}, v);
        return seg.emplace_hint(it, pair{x, r}, v);
    };

    auto operate = [&](u32 l, u32 r, u32 v)
    {
        auto ri = split(r), li = split(l);
        u32 res = 0, lst = 0;
        for (; li != ri; li = seg.erase(li)) {
            res ^= qpow(lst, li->second);
            lst = li->second;
            auto [l, r] = li->first;
            if (~(r - l) & 1) res ^= qpow(lst, lst);
        }
        seg.emplace_hint(ri, pair{l, r}, v);
        return res;
    };

    u32 l, r, v;
    while (m--) {
        cin >> l >> r >> v;
        cout << operate(l, r, v) << '\n';
    }
    return 0;
}