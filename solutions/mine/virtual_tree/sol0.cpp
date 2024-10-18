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

template<typename D, typename L>
vec<pair<u32, u32>> virtual_tree(vec<u32> t, const D &dfn, L &&lca)
{
    if (t.empty()) return {};
    sort(t.begin(), t.end(), [&dfn](u32 x, u32 y) { return dfn[x] < dfn[y]; });
    for (u32 i = 0, m = t.size(); i + 1 < m; i++) {
        t.emplace_back(lca(t[i], t[i + 1]));
    }
    sort(t.begin(), t.end(), [&dfn](u32 x, u32 y) { return dfn[x] < dfn[y]; });
    u32 m = unique(t.begin(), t.end()) - t.begin();
    vec<pair<u32, u32>> res(m);
    res[0] = {t[0], -1u};
    for (u32 i = 1; i < m; i++) {
        res[i] = {t[i], lca(t[i], t[i - 1])};
    }
    return res;
}

signed main() 
{
    return 0;
}