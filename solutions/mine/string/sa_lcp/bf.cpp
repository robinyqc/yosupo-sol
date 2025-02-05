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

u32 lcp(const string &a, const string &b)
{
    u32 i = 0;
    for (; i < min(a.size(), b.size()); i++) {
        if (a[i] != b[i]) return i;
    }
    return i;
}

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    string s;
    cin >> s;

    u32 q;
    cin >> q;
    while (q--) {
        u32 x, y;
        cin >> x >> y;
        cout << lcp(s.substr(x), s.substr(y)) << endl;
    }
    return 0;
}