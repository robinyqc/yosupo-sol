// Calc lcp of substrings.
// Based on solutions/string/suffixarray/sol0.cpp

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

struct SuffixArray
{
    vec<u32> sa; 
    vec<u32> rk; 
    vec<u32> hi; // 注意这个 height 数组中, hi[i] = lcp(sa[i], sa[i + 1]).
    SuffixArray() { }
    SuffixArray(const string &s): 
        SuffixArray(s.size(), 256, [&](u32 i){return s[i];}) { }
    template<typename F>
    SuffixArray(u32 n, u32 sig, F &&s): sa(n), rk(n), hi(n)
    {
        sig = max(sig, n);
        vec<u32> buc(sig), ork(n * 2, -1u), id(n);

        for (u32 i = 0; i < n; i++) buc[rk[i] = s(i)]++;
        for (u32 i = 0; i + 1 < sig; i++) buc[i + 1] += buc[i];
        for (u32 i = n - 1; ~i; --i) sa[--buc[rk[i]]] = i;

        u32 w;
        auto comp = [&](u32 x, u32 y) 
        {
            return ork[x] == ork[y] && ork[x + w] == ork[y + w];
        };
        for (w = 1; w < n; w *= 2) {
            u32 p = 0;
            for (u32 i = n - w; i < n; i++) id[p++] = i;
            for (u32 i = 0; i < n; i++) {
                if (sa[i] >= w) id[p++] = sa[i] - w;
            }
            buc.assign(sig, 0);
            for (u32 i = 0; i < n; i++) buc[rk[i]]++;
            for (u32 i = 0; i + 1 < sig; i++) buc[i + 1] += buc[i];
            for (u32 i = n - 1; ~i; --i) {
                sa[--buc[rk[id[i]]]] = id[i];
            }
            memcpy(ork.data(), rk.data(), n * sizeof(u32));
            p = 0;
            for (u32 i = 0; i + 1 < n; i++) {
                rk[sa[i]] = p;
                if (!comp(sa[i], sa[i + 1])) ++p;
            }
            rk[sa[n - 1]] = p;
            if (p == n) break;
        }

        for (u32 i = 0, k = 0; i < n; i++) {
            if (rk[i] == n - 1) continue;
            if (k) --k;
            u32 temp = sa[rk[i] + 1], j = max(temp, i);
            while (j + k < n && s(i + k) == s(temp + k)) ++k;
            hi[rk[i]] = k;
        }
    }
};

template<typename RMQ>
struct SA_LCP: SuffixArray
{
    using B = SuffixArray;
    using B::sa;
    using B::rk;
    using B::hi;
    RMQ rmq;

    SA_LCP(): B::SuffixArray(), rmq() { }
    template<typename F>
    SA_LCP(u32 n, u32 sig, F &&f): B::SuffixArray(n, sig, f), 
        rmq(n, [&](u32 x){return hi[x];}) { }
    SA_LCP(const string &s): SA_LCP(s.size(), 256, [&](u32 x){return s[x];}) {}

    u32 lcp(u32 x, u32 y) 
    {
        if (x == y) return sa.size() - x;
        if (rk[x] > rk[y]) swap(x, y);
        return rmq.prod(rk[x], rk[y]);
    }
};

struct SparseTable
{
    vec<u32> raw;
    vec<u64> block;
    vec<vec<u32>> meta;

    template<typename F>
    SparseTable(u32 n, F &&rw): 
        raw(n), block(n + 1), 
        meta(32 - __builtin_clz((n + 63) >> 6))
    {
        for (u32 i = 0; i < n; i++) raw[i] = rw(i);
        u32 m = (n + 63) >> 6;
        u64 stk;
        for (u32 i = 0; i < m; i++) {
            u32 l = i << 6, r = min(n, (i + 1) << 6);
            stk = 0;
            for (u32 j = l; j < r; j++) {
                while (stk && raw[l + (63 ^ __builtin_clzll(stk))] >= raw[j]) {
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
                meta[j].emplace_back(min(meta[j - 1][i], 
                    meta[j - 1][i + (1 << (j - 1))]));
            }
        }
    }

    u32 prod_block(u32 l, u32 r)
    {
        return raw[__builtin_ctzll(block[r] >> (l & 63)) + l];
    }

    u32 prod_meta(u32 l, u32 r)
    {
        u32 len = 31 ^ __builtin_clz(r - l);
        return min(meta[len][l], meta[len][r - (1 << len)]);
    }

    u32 prod(u32 l, u32 r)
    {
        if ((l >> 6) == ((r - 1) >> 6)) return prod_block(l, r);
        u32 L = (l >> 6) + 1, R = r >> 6;
        u32 res = prod_block(l, L << 6); 
        if (r != (R << 6)) res = min(res, prod_block(R << 6, r));
        if (L == R) return res;
        return min(res, prod_meta(L, R));
    }
};

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    
    string s;
    cin >> s;

    SA_LCP<SparseTable> sa(s);

    u32 q;
    cin >> q;
    while (q--) {
        u32 x, y;
        cin >> x >> y;
        cout << sa.lcp(x, y) << endl;
    }
    return 0;
}