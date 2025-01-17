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

template<typename T = vec<u32>>
struct LCA
{
    T dfn;
    vec<vec<u32>> st;

    u32 smaller(u32 x, u32 y)
    {
        return dfn[x] < dfn[y] ? x : y;
    }

    template<typename S>
    LCA(u32 n, const T &_dfn, const S &fa): dfn(_dfn)
    {
        u32 m = __builtin_clz(n) ^ 31;
        st.resize(m + 1);
        st[0].resize(n);
        for (u32 i = 0; i < n; i++) st[0][dfn[i]] = fa[i];
        for (u32 i = 1, l = 1; i <= m; i++, l *= 2) {
            st[i].resize(n - l * 2 + 1);
            for (u32 j = 0, ed = n - l * 2; j <= ed; j++) {
                st[i][j] = smaller(st[i - 1][j], st[i - 1][j + l]);
            }
        }
    }

    u32 get_lca(u32 x, u32 y)
    {
        if (x == y) return x;
        if ((x = dfn[x]) > (y = dfn[y])) swap(x, y);
        u32 len = __builtin_clz(y - x) ^ 31;
        return smaller(st[len][x + 1], st[len][y - (1 << len) + 1]);
    }

    u32 operator()(u32 x, u32 y) { return get_lca(x, y); }
};

LCA<vec<u32>> build_lca(u32 n, const vec<vec<u32>> &t, u32 root = 0)
{
    u32 dfnn = 0;
    vec<u32> dfn(n), fa(n);
    
    fa[root] = root;

    [root, dfs = [&](auto &&f, u32 x) -> void
    {
        dfn[x] = dfnn++;
        for (auto v: t[x]) if (v != fa[x]) {
            fa[v] = x;
            f(f, v);
        }
    }](){dfs(dfs, root);}();

    return LCA<vec<u32>>(n, dfn, fa);
}

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, q;
    cin >> n >> q;
    vec<vec<u32>> t(n);
    for (u32 i = 1, f; i < n; i++) {
        cin >> f;
        t[f].emplace_back(i);
    }

    auto lca(build_lca(n, t));
    
    while (q--) {
        u32 x, y;
        cin >> x >> y;
        cout << lca(x, y) << '\n';
    }
    return 0;
}