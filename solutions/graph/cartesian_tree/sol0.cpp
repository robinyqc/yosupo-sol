#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <array>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

template<typename T, typename Comp = less<T>>
vec<u32> cartesian_tree(const vec<T> &a, Comp cmp = less<T>())
{
    u32 n = a.size();
    vec<u32> fa(n, -1), s(n + 1, -1);
    u32 t = 0;
    for (u32 i = 0; i < n; i++) {
        while (t && cmp(a[s[t - 1]], a[i])) s[t--] = -1;
        if (t) fa[i] = s[t - 1];
        if (~s[t]) fa[s[t]] = i;
        s[t++] = i;
    }
    return fa;
}

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n;
    cin >> n;
    vec<u32> a(n);
    for (u32 &i: a) cin >> i;
    a = cartesian_tree(a, greater());
    for (u32 i = 0; i < n; i++) {
        if (~a[i]) cout << a[i] << ' ';
        else cout << i << ' ';
    }
    cout << endl;
    return 0;
}