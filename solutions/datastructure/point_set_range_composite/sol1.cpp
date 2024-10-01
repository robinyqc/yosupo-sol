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

/**
 * @brief Segment tree with less memory usage. 
 * @details Exactly 2n M::S are generated.
 * @ref https://www.luogu.com/discuss/531072
 */
template<typename M>
struct CompressedSegmentTree
{
    using S = M::S;

    u32 n;
    vec<S> d;

    CompressedSegmentTree(u32 _n): n(_n), d(n * 2, M::un()) { }
    template<typename F>
    CompressedSegmentTree(u32 _n, F &&f): n(_n), d(2 * n, M::un())
    {
        build(f, 0, n, 1);
    }

    S prod(u32 l, u32 r) { return prod_rec(l, r, 0, n, 1); }
    S prod_all() { return d[1]; }
    void set(u32 x, const S &s) { set_rec(s, x, 0, n, 1); }
    S get(u32 x) { return get_rec(x, 0, n, 1); }

private:

    template<typename F>
    void build(F &&f, u32 l, u32 r, u32 x)
    {
        if (l + 1 == r) {
            d[x] = f(l);
            return ;
        }
        u32 mid = (l + r) / 2;
        build(f, l, mid, mid * 2);
        build(f, mid, r, mid * 2 + 1);
        d[x] = M::op(d[mid * 2], d[mid * 2 + 1]);
    }

    S prod_rec(u32 tl, u32 tr, u32 l, u32 r, u32 x)
    {
        if (tl <= l && r <= tr) return d[x];
        u32 mid = (l + r) / 2;
        S res{M::un()};
        if (tl < mid) res = prod_rec(tl, tr, l, mid, mid * 2);
        if (mid < tr) res = M::op(res, prod_rec(tl, tr, mid, r, mid * 2 + 1));
        return res;
    }

    void set_rec(const S &s, u32 tp, u32 l, u32 r, u32 x)
    {
        if (l + 1 == r) {
            d[x] = s;
            return ;
        }
        u32 mid = (l + r) / 2;
        if (tp < mid) set_rec(s, tp, l, mid, mid * 2);
        else set_rec(s, tp, mid, r, mid * 2 + 1);
        d[x] = M::op(d[mid * 2], d[mid * 2 + 1]);
    }

    S get_rec(u32 tp, u32 l, u32 r, u32 x)
    {
        if (l + 1 == r) return d[x];
        u32 mid = (l + r) / 2;
        if (tp < mid) return get_rec(tp, l, mid, mid * 2);
        return get_rec(tp, mid, r, mid * 2 + 1);
    }

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
    CompressedSegmentTree<Monoid> t(n, read_lf);
    u32 op, x, y, z;
    while (q--) {
        cin >> op >> x >> y >> z;
        if (op == 0) t.set(x, {y, z});
        else cout << calc(t.prod(x, y), z).val() << '\n';
    }
    return 0;
}