#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <atcoder/modint>
#include <atcoder/convolution>
#include <deque>
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
    u32 n, x;
    cin >> n;
    if (n == 0) {
        cout << 1 << endl;
        return 0;
    }
    deque<vec<mint>> a(n);
    for (auto &i: a) {
        cin >> x;
        i.resize(x + 1);
        for (auto &j: i) {
            cin >> x;
            j = x;
        }
    }
    while (a.size() > 1) {
        a.emplace_back(atcoder::convolution(a[0], a[1]));
        a.pop_front(), a.pop_front();
    }
    for (auto i: a[0]) cout << i.val() << ' ';
    cout << endl;
    return 0;
}