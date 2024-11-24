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

vec<u32> manacher(const string &s)
{
    u32 n = s.size() * 2 - 1;
    vec<u32> f(n);
    const auto t = [&s](u32 x)
    {
        if (x & 1) return char(-1); 
        return s[x >> 1];
    };
    for (u32 i = 0, r = 0, x = 0; i < n; i++) {
        if (i >= r) {
            r = 0;
            while (r <= i && i + r < n && t(i + r) == t(i - r)) r++;
            f[i] = r;
            x = i;
            r += i;
        }
        else {
            f[i] = min(f[2 * x - i], r - i);
            if (f[i] == r - i) {
                while (r < n && 2 * i >= r && t(r) == t(2 * i - r)) r++;
                x = i;
                f[i] = r - i;
            }
        }
    }
    for (u32 i = 0; i < n; i++) {
        if (i >= f[i] && i + f[i] < n) f[i]--;
    }
    return f;
}

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    string s;
    cin >> s;
    auto f(manacher(s));
    for (auto i: f) cout << i << ' ';
    cout << endl;
    return 0;
}