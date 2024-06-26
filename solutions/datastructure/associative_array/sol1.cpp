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
    int n;
    cin >> n;
    vec<pair<i64, i64>> qr(n);
    vec<i64> val;
    for (auto &[k, v]: qr) {
        cin >> v >> k;
        if (v == 0) {
            cin >> v;
            val.emplace_back(k);
        }
        else v = -1;
    }
    val.emplace_back(1.1e18);
    sort(val.begin(), val.end());
    val.erase(unique(val.begin(), val.end()), val.end());
    vec<i64> a(val.size());
    for (auto [k, v]: qr) {
        int p = lower_bound(val.begin(), val.end(), k) - val.begin();
        if (v == -1) {
            if (val[p] != k) cout << "0\n";
            else cout << a[p] << '\n';
        }
        else a[p] = v;
    }
    return 0;
}