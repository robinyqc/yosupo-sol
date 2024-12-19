// Using Maxflow (Dinic) to solve biparite matching.
// Complexity: O(m*sqrt(m))

#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

// need <limits> header.
template<typename T = u32>
class MaxFlow
{
public:

    MaxFlow() { }
    MaxFlow(u32 n): h(n, -1u) { }
    // rm: the space reserve for edges
    MaxFlow(u32 n, u32 rm): h(n, -1u)
    {
        e.reserve(rm * 2);
        cap.reserve(rm);
    }

    void add_edge(u32 u, u32 v, T c = numeric_limits<T>::max())
    {
        adde(u, v, c);
        adde(v, u, 0);
    }

    u32 esize() const { return e.size(); }

    // Calculate the max flow of the network.
    T max_flow(u32 _s, u32 _t)
    {
        T res = static_cast<T>(0);
        s = _s, t = _t;
        rem = cap;
        while (bfs()) {
            cur = h;
            res += dfs(t, numeric_limits<T>::max());
        }
        return res;
    }

    // The capacity of the id-th inserted edge.
    T capacity(u32 id) const { return cap[id >> 1]; }

    // The residual flow of the id-th inserted edge in the last max_flow call.
    T residual_flow(u32 id) const { return rem[id >> 1]; }

    // The flow of the id-th inserted edge in the last max_flow call.
    T flow(u32 id) const { return capacity(id) - residual_flow(id); }

private:
    u32 s, t;
    struct Edge 
    {
        u32 v, nxt;
        Edge(u32 _v, u32 _nxt): v(_v), nxt(_nxt) { }
    };
    vec<Edge> e;
    vec<T> cap;
    vec<u32> h, rem, dep, cur;

    void adde(u32 u, u32 v, T w)
    {
        e.emplace_back(v, h[u]);
        cap.emplace_back(w);
        h[u] = e.size() - 1;
    }

    bool bfs()
    {
        dep.assign(h.size(), -1u);

        vec<u32> q(h.size());
        u32 hd = 0, tl = 0;
        q[tl++] = s;
        dep[s] = 0;

        while (hd < tl) {
            u32 x = q[hd++], dv = dep[x] + 1;
            for (u32 i = h[x], v; ~i; i = e[i].nxt) {
                v = e[i].v;
                if (rem[i] != 0 && dep[v] == -1u) {
                    dep[v] = dv;
                    // 提前退出不会错，因为本来深度大于 t 的就不会流到 t。
                    if (v == t) return true;
                    q[tl++] = v;
                }
            }
        }
        return false;
    }

    // 从 t 到 s dfs 则所有无用边都不会遍历。
    T dfs(u32 x, T flow)
    {
        if (x == s || flow == 0) return flow;
        u32 v, dv = dep[x] - 1;
        T d, res = static_cast<T>(0);
        for (u32 &i = cur[x]; ~i; i = e[i].nxt) {
            v = e[i].v;
            if (dep[v] == dv && (d = dfs(v, min(flow - res, rem[i ^ 1])))) {
                res += d;
                rem[i] += d;
                rem[i ^ 1] -= d;
                if (res == flow) return res;
            }
        }
        return res;
    }
};

pair<u32, vec<u32>> bipartite_matching
    (u32 n, u32 m, const vec<pair<u32, u32>> &e)
{
    MaxFlow<> mf(n + m + 2, e.size() + n + m);
    for (auto [u, v]: e) mf.add_edge(u + 1, v + n + 1, 1);
    for (u32 i = 0; i < n; i++) mf.add_edge(0, i + 1, 1);
    for (u32 i = 0; i < m; i++) mf.add_edge(i + n + 1, n + m + 1, 1);
    u32 flow = mf.max_flow(0, n + m + 1);
    vec<u32> sch;
    for (u32 i = 0; i < e.size(); i++) {
        if (mf.residual_flow(i) == 0) sch.emplace_back(i);
    }
    return {flow, sch};
}

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, m, q;
    cin >> n >> m >> q;
    vec<pair<u32, u32>> e(q);
    for (auto &[u, v]: e) cin >> u >> v;
    auto [mf, sch] = bipartite_matching(n, m, e);
    cout << mf << endl;
    for (u32 i: sch) cout << e[i].first << ' ' << e[i].second << '\n';
    return 0;
}