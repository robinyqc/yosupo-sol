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

template<u32 None = -1u>
struct ODT
{
    u32 n;
    vec<u32> t;

    template<typename G>
    ODT(u32 _n, G &&g): n(_n)
    {
        u32 m = 1;
        while (m < n) m += m;
        t.resize(m * 2);
        build(g, 0, n, 1);
    }

    template<typename G>
    u32 build(G &&g, u32 l, u32 r, u32 x)
    {
        if (l + 1 == r) return t[x] = g(l);
        u32 mid = (l + r) >> 1;
        u32 rs = build(g, l, mid, x * 2);
        u32 ls = build(g, mid, r, x * 2 + 1);
        if (ls == rs) return t[x] = ls;
        return t[x] = None;
    }

    // 对于每个颜色段 [l, r]，调用 f(l, r, c)，c 是这个段在改为 v 之前的颜色。
    template<typename F>
    void assign(u32 l, u32 r, u32 v, F &&f)
    {
        auto &rt = t; // reference to t to be captured.
        u32 cl = l, cn = None;
        auto rec = [&cl, &cn, l, r, v, &f, &rt]
            (auto &&self, u32 nl, u32 nr, u32 x) mutable -> void
        {
            if (l <= nl && nr <= r && rt[x] != None) {
                if (rt[x] != cn) {
                    if (nl != l) f(cl, nl, cn);
                    cl = nl, cn = rt[x];
                }
                rt[x] = v;
                return ;
            }
            if (rt[x] != None) {
                rt[x * 2] = (rt[x * 2 + 1] = rt[x]);
            }
            u32 mid = (nl + nr) >> 1;
            if (l < mid) self(self, nl, mid, x * 2);
            if (mid < r) self(self, mid, nr, x * 2 + 1);
            // 有可能是 None，所以这里 rt[x] = v 是错的。
            if (rt[x * 2] == rt[x * 2 + 1]) rt[x] = rt[x * 2];
            else rt[x] = None;
        };
        rec(rec, 0, n, 1);
        f(cl, r, cn);
    }
};

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
    ODT<> t(n, [x = u32()](u32) mutable { return cin >> x, x; });
    
    u32 l, r, v;
    while (m--) {
        cin >> l >> r >> v;
        u32 res = 0, lst = 0;
        auto operate = [&](u32 nl, u32 nr, u32 col)
        {
            res ^= qpow(lst, col);
            lst = col;
            if (~(nr - nl) & 1) {
                res ^= qpow(lst, lst);
            }
        };
        t.assign(l, r, v, operate);
        cout << res << '\n';
    }
    return 0;
}