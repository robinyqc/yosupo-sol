#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <atcoder/fenwicktree>
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
    atcoder::fenwick_tree<i64> t(n);
    for (u32 i = 0, a; i < n; i++) {
        cin >> a;
        t.add(i, a);
    }
    int op, x, y;
    while (q--) {
        cin >> op >> x >> y;
        if (op == 0) t.add(x, y);
        else cout << t.sum(x, y) << '\n';
    }
    return 0;
}