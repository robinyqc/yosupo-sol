#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <tuple>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;


struct lf
{
    int k;
    i64 b; // k * x + b
    constexpr lf(int _k = 0, i64 _b = 3e18): k(_k), b(_b) { }
    constexpr i64 operator()(int x) const { return (i64)k * x + b; }
};

template<typename F, typename G, typename C = less<decltype(declval<F>()(0))>>
struct LiChaoSegmentTree
{
    using T = decltype(declval<F>()(0));

    u32 n, m;
    const G &dom;
    const C &comp;
    vec<F> t;

    LiChaoSegmentTree(u32 _n, const G &_dom, 
        const F &default_fn = F(), const C &_comp = C())
            : n(_n), m(1), dom(_dom), comp(_comp), t()
    {
        while (m < n) m *= 2;
        t.resize(m * 2, default_fn);
    }

    void add_func_at(u32 x, F f)
    {
        u32 hb = 31 ^ __builtin_clz(x);
        u32 l = (m >> hb) * x - m;
        u32 r = l + (m >> hb);
        while (l < r) {
            auto &g = t[x];
            T fl = invoke(f, l), fr = invoke(f, r - 1);
            T gl = invoke(g, l), gr = invoke(g, r - 1);
            bool bl = comp(fl, gl), br = comp(fr, gr);
            if (bl && br) {
                g = f;
                return ;
            }
            if (!bl && !br) return ;
            u32 mid = (l + r) / 2;
            T fm = invoke(f, mid), gm = invoke(g, mid);
            bool bm = comp(fm, gm);
            if (bm) {
                swap(g, f);
                if (!bl) x = x * 2, r = mid;
                else x = x * 2 + 1, l = mid;
            }
            else {
                if (bl) x = x * 2, r = mid;
                else x = x * 2 + 1, l = mid;
            }
        }
    }

    void add_func(const F &f)
    {
        if (n) add_func_at(1, f);
    }

    void add_func(const F &f, u32 l, u32 r)
    {
        for (l += m, r += m; l < r; l /= 2, r /= 2) {
            if (l & 1) add_func_at(l++, f);
            if (r & 1) add_func_at(--r, f);
        }
    }

    T query(u32 x)
    {
        T v = invoke(t[0], x);
        for (u32 p = x + m; p; p /= 2) {
            T w = invoke(t[p], x);
            if (comp(w, v)) v = w;
        }
        return v;
    }

    T invoke(const F &f, u32 x) { return f(dom(min(x, n - 1))); }
};

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, q;
    cin >> n >> q;
    vec<pair<int, i64>> qr;
    qr.reserve(n + q);
    i64 op, a, b;
    for (u32 i = 0; i < n; i++) {
        cin >> a >> b;
        qr.emplace_back(a, b);
    }
    vec<int> p;
    for (u32 i = 0; i < q; i++) {
        cin >> op;
        if (op == 0) {
            cin >> a >> b;
            qr.emplace_back(a, b);
        }
        else {
            cin >> a;
            qr.emplace_back(a, -3e18);
            p.emplace_back(a);
        }
    }
    sort(p.begin(), p.end());
    p.erase(unique(p.begin(), p.end()), p.end());
    LiChaoSegmentTree t(p.size(), [&p](u32 x) { return p[x]; }, lf());
    for (auto [x, y]: qr) {
        if (y == -3e18) {
            x = lower_bound(p.begin(), p.end(), x) - p.begin();
            cout << t.query(x) << '\n';
        }
        else {
            t.add_func(lf(x, y));
        }
    }
    return 0;
}