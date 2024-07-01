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

template <typename M>
struct FenwickTree
{
    using S = typename M::S;
    u32 n;
    vec<S> t;

    FenwickTree(u32 _n): n(_n), t(n + 1) { }
    template<typename G>
    FenwickTree(u32 _n, G &&g): n(_n), t()
    {
        t.reserve(n + 1);
        t.emplace_back(M::un());
        for (u32 i = 1; i <= n; i++) {
            t.emplace_back(M::op(t.back(), g(i - 1)));
        }
        for (u32 i = n; i >= 1; i--) {
            t[i] = M::op(t[i], M::iv(t[i - (i & -i)]));
        }
    }

    void add(u32 x, S d)
    {
        for (++x; x <= n; x += x & -x) t[x] = M::op(t[x], d);
    }
    S ask(u32 r)
    {
        S res = M::un();
        for (; r; r -= r & -r) res = M::op(res, t[r]);
        return res;
    }
    S ask(u32 l, u32 r)
    {
        return M::op(ask(r), M::iv(ask(l)));
    }
};

struct Monoid
{
    using S = i64;
    static constexpr S op(S a, S b) { return a + b; }
    static constexpr S iv(S a) { return -a; }
    static constexpr S un() { return 0; }
};

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, q;
    cin >> n >> q;
    vec<i64> a(n);
    for (auto &i: a) cin >> i;
    FenwickTree<Monoid> t(n, [&a](u32 i){ return a[i]; });
    int op, x, y;
    while (q--) {
        cin >> op >> x >> y;
        if (op == 0) t.add(x, y);
        else cout << t.ask(x, y) << '\n';
    }
    return 0;
}