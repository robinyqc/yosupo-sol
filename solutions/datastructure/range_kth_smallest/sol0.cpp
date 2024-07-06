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

template<typename M, int dep = 31> struct PersistentSegmentTree
{
    using S = typename M::S;
    struct Node
    {
        S val;
        u32 ls, rs;
        Node(const S &_val = M::un(), u32 _ls = 0, u32 _rs = 0)
            : val(_val), ls(_ls), rs(_rs) { }
    };
    vec<Node> t; ///< pool
    u32 new_node()
    {
        t.emplace_back();
        return t.size() - 1;
    }
    void reserve(u32 n) { t.reserve(n); }
    vec<u32> root;

    PersistentSegmentTree(): t(1, Node()), root(1, 0) { }

    u32 set(u32 p, u32 x, const S& v)
    {
        static u32 vis[dep];
        root.emplace_back(new_node());
        p = root[p];
        u32 q = root.back();
        for (int d = dep - 1; d >= 0; d--) {
            vis[d] = q;
            if ((x >> d) & 1) {
                t[q].ls = t[p].ls;
                q = (t[q].rs = new_node());
                p = t[p].rs;
            }
            else {
                t[q].rs = t[p].rs;
                q = (t[q].ls = new_node());
                p = t[p].ls;
            }
        }
        t[q].val = v;
        for (u32 pass: vis) push_up(pass);
        return root.size() - 1;
    }

    S get(u32 p, u32 x)
    {
        p = root[p];
        for (int d = dep - 1; d >= 0; d--) {
            if ((x >> d) & 1) p = t[p].rs;
            else p = t[p].ls;
        }
        return t[p].val;
    }

    S prod(u32 p, u32 l, u32 r)
    {
        return prod_rec(l, r, 0, 1u << dep, root[p]);
    }

    template<typename F, typename G>
    u32 max_right(u32 p, u32 q, const F &diff, const G &chk)
    {
        p = root[p], q = root[q];
        S vl = M::un();
        u32 res = 0;
        for (int d = dep - 1; d >= 0; d--) {
            S vn = diff(t[t[q].ls].val, t[t[p].ls].val);
            if (chk(M::op(vl, vn))) {
                vl = M::op(vl, vn);
                p = t[p].rs;
                q = t[q].rs;
                res = res * 2 + 1;
            }
            else {
                p = t[p].ls;
                q = t[q].ls;
                res = res * 2;
            }
        }
        return res;
    }

    void push_up(u32 x) { t[x].val = M::op(t[t[x].ls].val, t[t[x].rs].val); }

    S prod_rec(u32 l, u32 r, u32 nl, u32 nr, u32 p)
    {
        if (p == 0) return M::un();
        if (l <= nl && nr <= r) return t[p].val;
        u32 mid = (nl + nr) / 2;
        S res = M::un();
        if (l < mid) res = M::op(res, prod_rec(l, r, nl, mid, t[p].ls));
        if (mid < r) res = M::op(res, prod_rec(l, r, mid, nr, t[p].rs));
        return res;
    }
};

struct Monoid
{
    using S = u32;
    static constexpr S op(S a, S b) { return a + b; }
    static constexpr S un() { return 0; }
};

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, q;
    cin >> n >> q;
    vec<u32> a(n);
    for (auto &i: a) cin >> i;
    vec<u32> v(a);
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    for (auto &i: a) i = lower_bound(v.begin(), v.end(), i) - v.begin();
    PersistentSegmentTree<Monoid, 18> pt;
    vec<u32> cnt(v.size());
    for (u32 i = 0; i < n; i++) {
        pt.set(i, a[i], ++cnt[a[i]]);
    }
    u32 l, r, k;
    while (q--) {
        cin >> l >> r >> k;
        cout << v[
            pt.max_right(l, r, 
            [](u32 x, u32 y) { return x - y; }, 
            [k](u32 x) { return x <= k; })
        ] << '\n';
    }
    return 0;
}