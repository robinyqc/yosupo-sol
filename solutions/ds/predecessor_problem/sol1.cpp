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

struct BITSet
{
    u32 n, m;
    vec<u32> t;

    BITSet(const vec<bool> &s): n(s.size()), m(1), t(n + 1)
    {
        while (m * 2 <= n) m *= 2;
        for (int i = 0; i < n; i++) t[i + 1] = t[i] + s[i];
        for (int i = n; i > 0; i--) t[i] = t[i] - t[i - (i & -i)];
    }

    void insert(u32 x) { for (++x; x <= n; x += x & -x) t[x]++; }
    void erase(u32 x)  { for (++x; x <= n; x += x & -x) t[x]--; }
    // number of elements less than x
    u32 rank(u32 x)
    {
        u32 res = 0;
        for (; x; x -= x & -x) res += t[x];
        return res; 
    }
    u32 operator[](u32 k)
    {
        u32 p = 0, s = 0;
        for (u32 i = m; i; i /= 2) {
            if (p + i <= n && s + t[p + i] <= k) {
                p += i;
                s += t[p];
            }
        }
        return p;
    }
};

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    int n, q;
    cin >> n >> q;
    vec<bool> t(n);
    char ch;
    for (int i = 0; i < n; i++) {
        cin >> ch;
        t[i] = ch - '0';
    }
    BITSet s(t);
    int op, k;
    while (q--) {
        cin >> op >> k;
        if (op == 0) {
            if (!t[k]) {
                t[k] = true;
                s.insert(k);
            }
        }
        else if (op == 1) {
            if (t[k]) {
                t[k] = false;
                s.erase(k);
            }
        }
        else if (op == 2) {
            cout << t[k] << '\n';
        }
        else if (op == 3) {
            auto it = s[s.rank(k)];
            if (it != n) cout << it << '\n';
            else cout << "-1\n";
        }
        else {
            u32 rk = s.rank(k + 1);
            if (rk == 0) cout << "-1\n";
            else cout << s[rk - 1] << '\n';
        }
    }
    return 0;
}