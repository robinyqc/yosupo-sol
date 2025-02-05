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
            while (i + k < n && s(i + k) == s(sa[rk[i] + 1])) ++k;
            hi[rk[i]] = k;
        }
    }
};

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    
    string s;
    cin >> s;
    SuffixArray sa(s);

    for (auto i: sa.sa) cout << i << ' ';
    cout << endl;
    return 0;
}