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

// M: only need S and op(S, S).
// op(a: S, b: S) -> S, and for a: S, op(a, a) = a.
template<typename M>
struct SparseTable
{
    using S = typename M::S;
    vec<vec<S>> t;

    template<typename F>
    SparseTable(u32 n, F &&f): t(32 - __builtin_clz(n))
    {
        t[0].reserve(n);
        for (u32 i = 0; i < n; i++) t[0].emplace_back(f(i));
        for (u32 j = 1; j < t.size(); j++) {
            t[j].reserve(n - (1 << j) + 1);
            for (u32 i = 0; i + (1 << j) <= n; i++) {
                t[j].emplace_back(M::op(t[j - 1][i], 
                                        t[j - 1][i + (1 << (j - 1))]));
            }
        }
    }

    S prod(u32 l, u32 r) 
    {
        u32 len = 31 ^ __builtin_clz(r - l);
        return M::op(t[len][l], t[len][r - (1 << len)]);
    }
};

struct Monoid
{
    using S = int;
    static constexpr S op(S a, S b) { return min(a, b); }
};

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, q;
    cin >> n >> q;
    SparseTable<Monoid> st(n, [x = int()](int)mutable{ return cin >> x, x; });
    u32 l, r;
    while (q--) {
        cin >> l >> r;
        cout << st.prod(l, r) << '\n';
    }
    return 0;
}