#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <atcoder/modint>
#include <atcoder/segtree>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

template<typename M>
struct SegmentTree
{
    using S = typename M::S;
    u32 m;
    vec<S> t;
    SegmentTree() { }
    SegmentTree(u32 n): m(1)
    {
        while (m < n) m *= 2;
        t.resize(m * 2, M::un());
    }
    template<typename G>
    SegmentTree(u32 n, G &&g): m(1)
    {
        while (m < n) m *= 2;
        t.resize(m * 2, M::un());
        for (u32 i = 0; i < n; i++) t[i + m] = g(i);
        for (u32 i = m - 1; i; i--) {
            t[i] = M::op(t[i * 2], t[i * 2 + 1]);
        }
    }

    void set(u32 x, S v)
    {
        t[x += m] = v;
        for (x /= 2; x; x /= 2) {
            t[x] = M::op(t[x * 2], t[x * 2 + 1]);
        }
    }

    S prod(u32 l, u32 r)
    {
        S lres = M::un(), rres = M::un();
        for (l += m, r += m; l < r; l /= 2, r /= 2) {
            if (l & 1) lres = M::op(lres, t[l++]);
            if (r & 1) rres = M::op(t[--r], rres);
        }
        return M::op(lres, rres);
    }

    S prod_all() { return t[1]; }
};

using mint = atcoder::modint998244353;

struct Monoid
{
    using S = pair<mint, mint>;
    static S op(const S &a, const S &b) 
    {
        return S(a.first * b.first, a.second * b.first + b.second);
    }
    static S un() { return S(1, 0); }
};
mint calc(const pair<mint, mint> &a, mint b) { return a.first * b + a.second; }

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, q;
    cin >> n >> q;
    auto read_lf = [x = int(), y = int()](int) mutable
    {
        cin >> x >> y;
        return Monoid::S(x, y);
    };
    SegmentTree<Monoid> t(n, read_lf);
    u32 op, x, y, z;
    while (q--) {
        cin >> op >> x >> y >> z;
        if (op == 0) t.set(x, {y, z});
        else cout << calc(t.prod(x, y), z).val() << '\n';
    }
    return 0;
}