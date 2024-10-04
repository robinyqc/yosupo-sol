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

/**
 * @brief Lazy segment tree with less memory usage. 
 * @details Exactly 2n M::MS::S and 2n M::MA::S are generated.
 * @ref https://www.luogu.com/discuss/531072
 */
template<typename M>
struct CompressedLazySegmentTree
{
    using MA = typename M::MA;
    using MS = typename M::MS;
    using A = typename MA::S;
    using S = typename MS::S;

    u32 n;
    vec<S> d;
    vec<A> t;

    CompressedLazySegmentTree(u32 _n): n(_n), d(2 * n), t(2 * n, MA::un()) { }
    template<typename F>
    CompressedLazySegmentTree(u32 _n, F &&f)
        : n(_n), d(2 * n), t(2 * n, MA::un())
    {
        build(f, 0, n, 1);
    }

    void apply(u32 l, u32 r, const A &a) { apply_rec(a, l, r, 0, n, 1); }
    S prod(u32 l, u32 r) { return prod_rec(l, r, 0, n, 1); }
    S prod_all() { return d[1]; }
    void set(u32 x, const S &s) { set_rec(s, x, 0, n, 1); }
    S get(u32 x) { return get_rec(x, 0, n, 1); }

private:

    void apply_at(u32 x, u32 len, const A &a)
    {
        d[x] = M::act(d[x], a, len);
        if (len > 1) t[x] = MA::op(t[x], a);
    }

    void push_down(u32 x, u32 l, u32 r, u32 mid)
    {
        if (t[x] == MA::un()) return ;
        apply_at(mid * 2, mid - l, t[x]);
        apply_at(mid * 2 + 1, r - mid, t[x]);
        t[x] = MA::un();
    }

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
        d[x] = MS::op(d[mid * 2], d[mid * 2 + 1]);
    }

    void apply_rec(const A &a, u32 tl, u32 tr, u32 l, u32 r, u32 x)
    {
        if (tl <= l && r <= tr) {
            apply_at(x, r - l, a);
            return ;
        }
        u32 mid = (l + r) / 2;
        push_down(x, l, r, mid);
        if (tl < mid) apply_rec(a, tl, tr, l, mid, mid * 2);
        if (mid < tr) apply_rec(a, tl, tr, mid, r, mid * 2 + 1);
        d[x] = MS::op(d[mid * 2], d[mid * 2 + 1]);
    }

    S prod_rec(u32 tl, u32 tr, u32 l, u32 r, u32 x)
    {
        if (tl <= l && r <= tr) return d[x];
        u32 mid = (l + r) / 2;
        push_down(x, l, r, mid);
        S res{MS::un()};
        if (tl < mid) res = prod_rec(tl, tr, l, mid, mid * 2);
        if (mid < tr) res = MS::op(res, prod_rec(tl, tr, mid, r, mid * 2 + 1));
        return res;
    }

    void set_rec(const S &s, u32 tp, u32 l, u32 r, u32 x)
    {
        if (l + 1 == r) {
            d[x] = s;
            return ;
        }
        u32 mid = (l + r) / 2;
        push_down(x, l, r, mid);
        if (tp < mid) set_rec(s, tp, l, mid, mid * 2);
        else set_rec(s, tp, mid, r, mid * 2 + 1);
        d[x] = MS::op(d[mid * 2], d[mid * 2 + 1]);
    }

    S get_rec(u32 tp, u32 l, u32 r, u32 x)
    {
        if (l + 1 == r) return d[x];
        u32 mid = (l + r) / 2;
        push_down(x, l, r, mid);
        if (tp < mid) return get_rec(tp, l, mid, mid * 2);
        return get_rec(tp, mid, r, mid * 2 + 1);
    }

};

using mint = atcoder::modint998244353;

struct MonoidLF
{
    using S = pair<mint, mint>;
    static S op(const S &a, const S &b) 
    {
        return S(a.first * b.first, a.second * b.first + b.second);
    }
    static S un() { return S(1, 0); }
};

struct MonoidAdd
{
    using S = mint;
    static S op(S a, S b) { return a + b; }
    static S un() { return 0; }
};

struct ActedMonoid
{
    using MA = MonoidLF;
    using MS = MonoidAdd;
    static mint act(mint v, const pair<mint, mint>& a, u32 len)
    {
        return a.first * v + a.second * len;
    }
};


signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, q;
    cin >> n >> q;
    auto read_int = [x = int()](u32)mutable{ return cin >> x, x; };
    CompressedLazySegmentTree<ActedMonoid> sgt(n, read_int);
    u32 op, l, r, a, b;
    while (q--) {
        cin >> op >> l >> r;
        if (op == 0) {
            cin >> a >> b;
            sgt.apply(l, r, {a, b});
        }
        else {
            cout << sgt.prod(l, r).val() << '\n';
        }
    }
    return 0;
}