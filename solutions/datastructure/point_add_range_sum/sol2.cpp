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

template<typename T, u32 B>
struct RQArray
{
    vec<T> t, b;
    RQArray(u32 n): t(n), b((n - 1) / B + 1) { }
    RQArray(vec<T> s): t(std::move(s)), b((t.size() - 1) / B + 1)
    {
        for (u32 i = 0; i < t.size(); i++) b[i / B] += t[i];
    }

    void add(u32 x, T y)
    {
        t[x] += y;
        b[x / B] += y;
    }

    T ask(u32 x)
    {
        T res = 0;
        u32 i = 0;
        for (i = 0; (i + 1) * B <= x; i++) res += b[i];
        i *= B;
        for (; i < x; i++) res += t[i];
        return res;
    }

    T ask(u32 l, u32 r)
    {
        return ask(r) - ask(l);
    }
};

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, q;
    cin >> n >> q;
    vec<i64> a(n);
    for (auto &i: a) cin >> i;
    RQArray<i64, 700> t(std::move(a));
    int op, x, y;
    while (q--) {
        cin >> op >> x >> y;
        if (op == 0) t.add(x, y);
        else cout << t.ask(x, y) << '\n';
    }
    return 0;
}