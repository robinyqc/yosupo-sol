#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <bitset>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

namespace w_ary_trie
{

u64 f0, f1[64], f2[64 * 64], f3[64 * 64 * 64];

void build(const vec<bool> &t)
{
    for (u32 i = 0, n = t.size(); i < n; i++) {
        f3[i >> 6] |= (u64)t[i] << (i & 63);
    }
    for (u32 i = 0; i < 64 * 64 * 64; i++) f2[i >> 6] |= (u64)(bool)f3[i] << (i & 63);
    for (u32 i = 0; i < 64 * 64; i++) f1[i >> 6] |= (u64)(bool)f2[i] << (i & 63);
    for (u32 i = 0; i < 64; i++) f0 |= (u64)(bool)f1[i] << i;
}

bool count(u32 x) { return (f3[x >> 6] >> (x & 63)) & 1; }

void insert(u32 x) 
{
    f3[x >> 6] |= 1ull << (x & 63), x >>= 6;
    f2[x >> 6] |= 1ull << (x & 63), x >>= 6;
    f1[x >> 6] |= 1ull << (x & 63), x >>= 6;
    f0 |= 1ull << (x & 63);
}

void erase(u32 x)
{
    if (!count(x)) return ;
    if (!(f3[x >> 6] ^= 1ull << (x & 63))) {
        x >>= 6;
        if (!(f2[x >> 6] ^= 1ull << (x & 63))) {
            x >>= 6;
            if (!(f1[x >> 6] ^= 1ull << (x & 63))) {
                x >>= 6;
                f0 ^= 1ull << (x);
            }
        }
    }
}

u32 next(u32 x)
{
    if (count(x)) return x;
    u32 q = x & 63;
    x >>= 6;
    if ((f3[x] >> q) <= 1) {
        q = x & 63, x >>= 6;
        if ((f2[x] >> q) <= 1) {
            q = x & 63, x >>= 6;
            if ((f1[x] >> q) <= 1) {
                q = x & 63;
                if ((f0 >> q) <= 1) return -1u;
                x = __builtin_ctzll(f0 >> (q + 1)) + q + 1, q = -1;
            }
            x = x << 6 | (__builtin_ctzll(f1[x] >> (q + 1)) + q + 1), q = -1;
        }
        x = x << 6 | (__builtin_ctzll(f2[x] >> (q + 1)) + q + 1), q = -1;
    }
    return x << 6 | (__builtin_ctzll(f3[x] >> (q + 1)) + q + 1);
}

u32 prev(u64 x)
{
    if (count(x)) return x;
    u64 q = x & 63;
    x >>= 6;
    if (!(f3[x] & ((1ull << q) - 1))) {
        q = x & 63, x >>= 6;
        if (!(f2[x] & ((1ull << q) - 1))) {
            q = x & 63, x >>= 6;
            if (!(f1[x] & ((1ull << q) - 1))) {
                q = x & 63;
                if (!(f0 & ((1ull << q) - 1))) return -1;
                x = 63 ^ __builtin_clzll(f0 & ((1ull << q) - 1));
                q = -1;
            }
            else q = (1ull << q) - 1;
            x = x << 6 | (63 ^ __builtin_clzll(f1[x] & q));
            q = -1;
        }
        else q = (1ull << q) - 1;
        x = x << 6 | (63 ^ __builtin_clzll(f2[x] & q));
        q = -1;
    }
    else q = (1ull << q) - 1;
    return x << 6 | (63 ^ __builtin_clzll(f3[x] & q));
}

}

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, q;
    cin >> n >> q;
    {
        vec<bool> t(n);
        char ch;
        for (u32 i = 0; i < n; i++) {
            cin >> ch;
            t[i] = ch - '0';
        }
        w_ary_trie::build(t);
    }
    u32 op, k;
    while (q--) {
        cin >> op >> k;
        if (op == 0) {
            w_ary_trie::insert(k);
        }
        else if (op == 1) {
            w_ary_trie::erase(k);
        }
        else if (op == 2) {
            cout << w_ary_trie::count(k) << '\n';
        }
        else if (op == 3) {
            auto it = w_ary_trie::next(k);
            if (~it) cout << it << '\n';
            else cout << "-1\n";
        }
        else {
            auto it = w_ary_trie::prev(k);
            if (~it) cout << it << '\n';
            else cout << "-1\n";
        }
    }
    return 0;
}