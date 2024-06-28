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

// only for min
// O(n) - O(1)
template<typename T, typename Comp>
struct RMQSparseTable
{
    static constexpr Comp cmp = Comp();
    struct Monoid
    {
        using S = T;
        static constexpr S op(const S &a, const S &b) 
        {
            if (cmp(a, b)) return a;
            return b;
        }
    };
    vec<T> raw;
    vec<u64> block;
    // SparseTable<Monoid> meta;

    template<typename F>
    RMQSparseTable(u32 n, F &&f): raw(std::move(from_lambda(n, f))), block(std::move(build_block(n))) { }

    template<typename F>
    vec<T> from_lambda(u32 n, F &&f)
    {
        vec<T> res;
        res.reserve(n);
        for (u32 i = 0; i < n; i++) res.emplace_back(f(i));
        return std::move(res);
    }

    vec<u64> build_block(u32 n)
    {
        vec<u64> res(n + 1);
        u64 stk;
        for (u32 i = 0; i < n; i++) {
            u32 l = i << 6, r = min(n, (i + 1) << 6);
            stk = 0;
            for (u32 j = l; j < r; j++) {
                while (stk && !cmp(raw[l + (63 ^ __builtin_clzll(stk))], raw[j])) {
                    stk ^= 1ull << (63 ^ __builtin_clzll(stk));
                }
                stk ^= 1ull << (j - l);
                res[j + 1] = stk;
            }
        }
        return res;
    }
};

signed main() 
{
    RMQSparseTable<int, less<>> st(1, [](int){return 1;});
    return 0;
}