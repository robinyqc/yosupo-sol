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

template<typename M>
struct LazySegmentTree
{
    using MA = typename M::MA;
    using MS = typename M::MS;
    using A = typename MA::S;
    using S = typename MS::S;

    u32 log;
    vec<S> d; // data
    vec<A> t; // tag

#define __m (1u << log)

    template<typename G>
    LazySegmentTree(u32 n, G &&g): log(0), t()
    {
        while (__m < n) ++log;
        d.resize(__m * 2, MS::un());
        t.resize(__m, MA::un());
        for (u32 i = 0; i < n; i++) d[i + __m] = g(i);
        for (u32 i = __m - 1; i; i--) push_up(i);
    }

    void push_up(u32 x) { d[x] = MS::op(d[x * 2], d[x * 2 + 1]); }

    void apply_at(u32 x, const A &a)
    {
        u32 len = 1u << (log - (31 ^ __builtin_clz(x)));
        d[x] = M::act(d[x], a, len);
        if (x < __m) t[x] = MA::op(t[x], a);
    }

    void push_down(u32 x) 
    {
        if (t[x] == MA::un()) return ;
        apply_at(x * 2, t[x]);
        apply_at(x * 2 + 1, t[x]);
        t[x] = MA::un();
    }

    void set(u32 x, const S &v)
    {
        x += __m;
        for (u32 i = log; i; --i) push_down(x >> i);
        d[x] = v;
        for (x /= 2; x; x /= 2) push_up(x);
    }

    S get(u32 x)
    {
        x += __m;
        for (u32 i = log; i; --i) push_down(x >> i);
        return d[x];
    }

    S prod(u32 l, u32 r)
    {
        if (l == r) return MS::un();
        l += __m, r += __m;
        for (u32 i = log; i; --i) {
            if (((l >> i) << i) != l) push_down(l >> i);
            if (((r >> i) << i) != r) push_down((r - 1) >> i);
        }
        S lres = MS::un(), rres = MS::un();
        for (; l < r; l /= 2, r /= 2) {
            if (l & 1) lres = MS::op(lres, d[l++]);
            if (r & 1) rres = MS::op(d[--r], rres);
        }
        return MS::op(lres, rres);
    }

    void apply(u32 l, u32 r, const A &a)
    {
        if (l == r) return ;
        l += __m, r += __m;
        for (u32 i = log; i; --i) {
            if (((l >> i) << i) != l) push_down(l >> i);
            if (((r >> i) << i) != r) push_down((r - 1) >> i);
        }
        
        for (u32 tl = l, tr = r; tl < tr; tl /= 2, tr /= 2) {
            if (tl & 1) apply_at(tl++, a);
            if (tr & 1) apply_at(--tr, a);
        }

        for (u32 i = 1; i <= log; ++i) {
            if (((l >> i) << i) != l) push_up(l >> i);
            if (((r >> i) << i) != r) push_up((r - 1) >> i);
        }
    }

#undef __m
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
    static mint act(mint v, const pair<mint, mint> & a, u32 len)
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
    LazySegmentTree<ActedMonoid> sgt(n, read_int);
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