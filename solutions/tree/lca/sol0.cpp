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

struct TreeDecomposition
{
    vec<u32> fa, dep, top, dfn, dfnr, rnk;

    static constexpr auto identity = [](u32 x){return x;};

    template<typename G, typename F = decltype(identity)> 
    TreeDecomposition(u32 n, const G &g, u32 r = 0, F get_v = identity)
        : fa(n), dep(n), top(n), dfn(n), dfnr(n), rnk(n)
    {
        vec<u32> son(n, -1);

        fa[r] = r;
        [r, dfs = [&](auto &&f, u32 x) -> void
        {
            dfn[x] = 1;
            for (const auto &i: g[x]) {
                u32 v = get_v(i);
                if (v != fa[x]) {
                    fa[v] = x;
                    dep[v] = dep[x] + 1;
                    f(f, v);
                    dfn[x] += dfn[v];
                    if (son[x] == -1u || dfn[v] > dfn[son[x]]) son[x] = v;
                }
            }
        }](){dfs(dfs, r);}();

        u32 dfnn = 0;
        top[r] = r;

        [r, dfs = [&](auto &&f, u32 x) -> void
        {
            rnk[dfnn] = x;
            dfn[x] = dfnn++;
            if (~son[x]) {
                top[son[x]] = top[x];
                f(f, son[x]);
            }
            for (const auto &i: g[x]) {
                u32 v = get_v(i);
                if (v != fa[x] && v != son[x]) {
                    top[v] = v;
                    f(f, v);
                }
            }
            dfnr[x] = dfnn;
        }](){dfs(dfs, r);}();
    }

    u32 lca(u32 x, u32 y)
    {
        while (top[x] != top[y]) {
            if (dep[top[x]] < dep[top[y]]) y = fa[top[y]];
            else x = fa[top[x]];
        }
        return dep[x] < dep[y] ? x : y;
    }

    // 注意是左闭右闭，这样好描述 “路径” 这一过程。
    vec<pair<u32, u32>> path(u32 x, u32 y)
    {
        static vec<pair<u32, u32>> px, py;
        px.clear(), py.clear();

        while (top[x] != top[y]) {
            if (dep[top[x]] < dep[top[y]]) {
                py.emplace_back(dfn[top[y]], dfn[y]);
                y = fa[top[y]];
            }
            else {
                px.emplace_back(dfn[x], dfn[top[x]]);
                x = fa[top[x]];
            }
        }
        px.emplace_back(dfn[x], dfn[y]);
        for (auto it = py.rbegin(); it != py.rend(); ++it) {
            px.emplace_back(*it);
        }
        return px;
    }

    // 注意是左闭右开，因为这里不在乎边的顺序，所以左闭右开方便。
    template<typename F, typename... Args>
    void operate_path(u32 x, u32 y, F &&f, Args &&... args)
    {
        while (top[x] != top[y]) {
            if (dep[top[x]] < dep[top[y]]) swap(x, y);
            f(dfn[top[x]], dfn[x] + 1, std::forward<Args>(args)...);
            x = fa[top[x]];
        }
        if (dep[x] < dep[y]) swap(x, y);
        f(dfn[y], dfn[x] + 1, std::forward<Args>(args)...);
    }
};

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    int n, m;
    cin >> n >> m;
    vec<vec<int>> g(n);
    for (int i = 1, p; i < n; i++) {
        cin >> p;
        g[p].emplace_back(i);
    }

    TreeDecomposition td(n, g);

    while (m--) {
        int x, y;
        cin >> x >> y;
        cout << td.lca(x, y) << '\n';
    }
    return 0;
}