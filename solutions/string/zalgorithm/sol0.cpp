#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <cassert>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

vec<u32> z_function(const string &s)
{
    u32 n = s.size();
    vec<u32> z(n);
    z[0] = n;
    for (u32 i = 1, l = 0, r = 0; i < n; i++) {
        if (i >= r) {
            r = 0;
            while (i + r < n && s[i + r] == s[r]) r++;
            z[i] = r;
            l = i;
            r += i;
        }
        else {
            z[i] = min(r - i, z[i - l]);
            if (z[i] == r - i) {
                while (r < n && s[r] == s[r - i]) r++;
                z[i] = r - i;
                l = i;
            }
        }
    }
    return z;
}

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    string s;
    cin >> s;
    auto z(z_function(s));
    for (auto i: z) cout << i << ' ';
    cout << endl;
    return 0;
}