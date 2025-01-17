#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <random>
#include <ctime>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

struct EdgeInfo
{
    u64 wei;
    u32 dep, v;
    EdgeInfo(u64 w, u32 d, u32 _v): wei(w), dep(d), v(_v) { }
    bool operator<(const EdgeInfo &t) const
    {
        if (wei != t.wei) return wei < t.wei;
        return dep < t.dep;
    }
};

template<typename T>
struct Span
{
    T *b, *e;
    T *begin() const { return b; }
    T *end() const { return e; }
};

struct StaticGraph
{
    vec<pair<u32, u32>> e;
    vec<u32> s;

    StaticGraph() { }
    StaticGraph(u32 n, const vec<pair<u32, u32>> &es)
        : e(es.size() * 2), s(n + 1)
    {
        for (auto [u, v]: es) ++s[u], ++s[v];
        for (u32 i = 0; i < n; i++) s[i + 1] += s[i];
        for (u32 i = 0, m = es.size(); i < m; i++) {
            auto [u, v] = es[i];
            e[--s[u]] = {v, i};
            e[--s[v]] = {u, i};
        }
    }

    Span<pair<u32, u32>> operator[](u32 u)
    {
        return {e.data() + s[u], e.data() + s[u + 1]};
    }
};

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);

    u32 n, m;
    cin >> n >> m;

    StaticGraph g;

    {
        vec<pair<u32, u32>> e;
        e.reserve(m);
        for (u32 i = 0, u, v; i < m; i++) {
            cin >> u >> v;
            if (u == v) continue;
            e.emplace_back(u, v);
        }
        m = e.size();
        g = StaticGraph(n, e);
    }

    vec<bool> visv(n);
    vec<u64> val(n);
    vec<u32> ps(n), dep(n);
    vec<pair<u64, u32>> col(n);

    mt19937_64 gen(random_device{}() + time(0) + clock());

    vec<EdgeInfo> te;
    te.reserve(n - 1);

    [&, dfs = [&](auto &&f, u32 x, u32 fd) -> void
    {
        visv[x] = true;
        col[x].second = x;
        for (auto [v, id]: g[x]) {
            if (!visv[v]) {
                dep[v] = dep[x] + 1;
                f(f, v, id);
                u64 w = val[v];
                val[x] ^= w;
                te.emplace_back(w, dep[x], v);
                ps[x] += ps[v];
                if (ps[v] <= 1) col[v].first ^= gen();
            }
            else if (id != fd && dep[v] < dep[x]) {
                u64 w = gen();
                val[x] ^= w;
                val[v] ^= w;
                ps[x] += 1;
                ps[v] -= 1;
            }
        }
    }]()
    {
        for (u32 i = 0; i < n; i++) {
            if (!visv[i]) {
                col[i].first ^= gen();
                dfs(dfs, i, -1);
            }
        }
    }();

    sort(te.begin(), te.end());

    for (u32 i = 0, nn = te.size(); i + 1 < nn; i++) {
        if (te[i].wei == te[i + 1].wei) {
            u64 w = gen();
            col[te[i].v].first ^= w;
            col[te[i + 1].v].first ^= w;
        }
    }

    visv.assign(n, false);
    [&, dfs = [&](auto &&f, u32 x) -> void
    {
        visv[x] = true;
        for (auto [v, id]: g[x]) {
            if (!visv[v]) {
                col[v].first ^= col[x].first;
                f(f, v);
            }
        }
    }]()
    {
        for (u32 i = 0; i < n; i++) {
            if (!visv[i]) dfs(dfs, i);
        }
    }();

    sort(col.begin(), col.end());

    vec<vec<u32>> tec;
    vec<u32> nw;

    for (u32 i = 0; i < n; i++) {
        nw.emplace_back(col[i].second);
        if (i + 1 == n || col[i].first != col[i + 1].first) {
            tec.emplace_back(nw);
            nw.clear();
        }
    }

    cout << tec.size() << '\n';

    for (auto &i: tec) {
        cout << i.size() << ' ';
        for (auto j: i) cout << j << ' ';
        cout << '\n';
    }

    return 0;
}