#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <atcoder/segtree>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

constexpr int min_int(int a, int b) { return min(a, b); }
constexpr int min_unit() { return 1e9 + 1; }

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, q;
    cin >> n >> q;
    vec<int> a(n);
    for (int &i: a) cin >> i;
    atcoder::segtree<int, min_int, min_unit> t(a);
    u32 l, r;
    while (q--) {
        cin >> l >> r;
        cout << t.prod(l, r) << '\n';
    }
    return 0;
}