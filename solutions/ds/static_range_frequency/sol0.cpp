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

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, q;
    cin >> n >> q;
    vec<u32> a(n);
    for (auto &i: a) cin >> i;
    auto val(a);
    sort(val.begin(), val.end());
    val.erase(unique(val.begin(), val.end()), val.end());
    vec<vec<u32>> occur(val.size());
    for (u32 i = 0; i < n; i++) {
        u32 v = lower_bound(val.begin(), val.end(), a[i]) - val.begin();
        occur[v].emplace_back(i);
    }
    u32 l, r, x;
    while (q--) {
        cin >> l >> r >> x;
        if (n == 0) cout << "0\n";
        else {
            u32 vx = lower_bound(val.begin(), val.end(), x) - val.begin();
            if (val[vx] != x) cout << "0\n";
            else {
                auto pl = lower_bound(occur[vx].begin(), occur[vx].end(), l);
                auto pr = lower_bound(occur[vx].begin(), occur[vx].end(), r);
                cout << pr - pl << '\n';
            }
        }
    }
    return 0;
}