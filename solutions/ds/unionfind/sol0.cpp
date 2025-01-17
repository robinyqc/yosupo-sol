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

struct DSU
{
    vec<u32> fa;
    DSU(u32 n): fa(n) { for (u32 i = 0; i < n; i++) fa[i] = i; }
    u32 gtf(u32 x) { while (x != fa[x]) x = fa[x] = fa[fa[x]]; return x; }
    bool in_same(u32 x, u32 y) { return gtf(x) == gtf(y); }
    void merge(u32 x, u32 y)
    {
        x = gtf(x), y = gtf(y);
        if (x != y) fa[x] = y;
    }
};

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, q;
    cin >> n >> q;
    DSU d(n);
    for (u32 i = 0, t, u, v; i < q; i++) {
        cin >> t >> u >> v;
        if (t) cout << d.in_same(u, v) << '\n';
        else d.merge(u, v);
    }
    return 0;
}