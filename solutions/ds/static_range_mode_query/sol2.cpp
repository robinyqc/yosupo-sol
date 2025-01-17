#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <cmath>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);

    u32 n, q;
    cin >> n >> q;
    if (q == 0) return 0;
    u32 b = ceil(n / sqrt(q) * 0.8), m = (n + b - 1) / b;

    vec<u32> a(n);
    for (u32 &i: a) cin >> i;

    vec<u32> val(a);
    sort(val.begin(), val.end());
    val.erase(unique(val.begin(), val.end()), val.end());

    u32 V = val.size();
    vec<vec<u32>> ind(V); ///< ind[v] store all i that a[i] == v
    vec<u32> pos(n); ///< ind[a[x]][pos[x]] = x

    for (u32 i = 0; i < n; i++) {
        a[i] = lower_bound(val.begin(), val.end(), a[i]) - val.begin();
        pos[i] = ind[a[i]].size();
        ind[a[i]].emplace_back(i);
    }

    vec<vec<pair<u32, u32>>> meta(m);
    vec<u32> cnt(V);

    for (u32 i = 0; i * b < n; i++) {
        u32 mx = 0;
        meta[i].emplace_back(V, 0);
        for (u32 j = i + 1; j <= m; j++) {
            u32 l = (j - 1) * b, r = min(j * b, n);
            for (u32 k = l; k < r; k++) {
                if ((++cnt[a[k]]) > cnt[mx]) mx = a[k];
            }
            meta[i].emplace_back(mx, cnt[mx]);
        }
        cnt = vec<u32>(V);
    }

    u32 l, r;
    while (q--) {
        cin >> l >> r;
        if (r - l <= b) {
            u32 mx = 0;
            for (u32 k = l; k < r; k++) {
                if ((++cnt[a[k]]) > cnt[mx]) mx = a[k];
            }
            cout << val[mx] << ' ' << cnt[mx] << '\n';
            for (u32 k = l; k < r; k++) cnt[a[k]] = 0;
        }
        else {
            u32 lb = (l + b - 1) / b, rb = r / b;
            auto [mx, c] = meta[lb][rb - lb];
            for (u32 k = l; k < lb * b; k++) {
                u32 p = pos[k], v = a[k];
                while (p + c < ind[v].size() && ind[v][p + c] < r) mx = v, c++;
            }
            for (u32 k = rb * b; k < r; k++) {
                u32 p = pos[k], v = a[k];
                while (p >= c && ind[v][p - c] >= l) mx = v, c++;
            }
            cout << val[mx] << ' ' << c << '\n';
        }
    }

    return 0;
}