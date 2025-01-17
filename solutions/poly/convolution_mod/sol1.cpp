#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <atcoder/convolution>
#include <atcoder/modint>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;
using mint = atcoder::modint998244353;

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, m, x;
    cin >> n >> m;
    vec<mint> a(n), b(m);
    for (auto &i: a) cin >> x, i = x;
    for (auto &i: b) cin >> x, i = x;
    a = atcoder::convolution(a, b);
    for (auto i: a) cout << i.val() << ' ';
    cout << endl;
    return 0;
}