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

template<typename S> using vec = vector<S>;

// only for min
// O(n) - O(1)
template<typename F, typename Comp>
struct RMQSparseTable
{
    using S = decltype(declval<F>()(0));
    Comp cmp;
    const F &raw;
    vec<u64> block;
    vec<vec<S>> meta;

    // may be when Ofast op(x: S, y: S) will faster than op(x: const S&, y: const S&)
    S op(const S &x, const S &y)
    {
        if (cmp(x, y)) return x;
        return y;
    }

    RMQSparseTable(u32 n, const F &rw, const Comp _cmp): 
        cmp(_cmp), raw(rw), block(n + 1), 
        meta(32 - __builtin_clz((n + 63) >> 6))
    {
        u32 m = (n + 63) >> 6;
        u64 stk;
        for (u32 i = 0; i < m; i++) {
            u32 l = i << 6, r = min(n, (i + 1) << 6);
            stk = 0;
            for (u32 j = l; j < r; j++) {
                while (stk && !cmp(
                    raw(l + (63 ^ __builtin_clzll(stk))), 
                    raw(j)
                )) {
                    stk ^= 1ull << (63 ^ __builtin_clzll(stk));
                }
                stk ^= 1ull << (j - l);
                block[j + 1] = stk;
            }
        }
        meta[0].reserve(m);
        for (u32 i = 0; i < m; i++) {
            meta[0].emplace_back(prod_block(i << 6, min((i + 1) << 6, n)));
        }
        for (u32 j = 1; j < meta.size(); j++) {
            meta[j].reserve(m - (1 << j) + 1);
            for (u32 i = 0; i + (1 << j) <= m; i++) {
                meta[j].emplace_back(op(meta[j - 1][i], 
                    meta[j - 1][i + (1 << (j - 1))]));
            }
        }
    }

    S prod_block(u32 l, u32 r)
    {
        return raw(__builtin_ctzll(block[r] >> (l & 63)) + l);
    }

    S prod_meta(u32 l, u32 r)
    {
        u32 len = 31 ^ __builtin_clz(r - l);
        return op(meta[len][l], meta[len][r - (1 << len)]);
    }

    S prod(u32 l, u32 r)
    {
        if ((l >> 6) == ((r - 1) >> 6)) return prod_block(l, r);
        u32 L = (l >> 6) + 1, R = r >> 6;
        S res = prod_block(l, L << 6); 
        if (r != (R << 6)) res = op(res, prod_block(R << 6, r));
        if (L == R) return res;
        return op(res, prod_meta(L, R));
    }
};

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, q;
    cin >> n >> q;
    vec<int> a(n);
    for (int &i: a) cin >> i;
    RMQSparseTable st(n, [&a](u32 i) { return a[i]; }, less());
    u32 l, r;
    while (q--) {
        cin >> l >> r;
        cout << st.prod(l, r) << '\n';
    }
    return 0;
}