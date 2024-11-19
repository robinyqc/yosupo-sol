// Leafy K-D Tree 

#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <tuple>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

/**
 * @brief K-Dimension Tree template implementation.
 * 
 * @tparam PT Type of the point in KD. 
 *            It must be tuple-like and have operator <=.
 *            std::pair, std::tuple and std::array fit the requirements.
 * @tparam M Commutative Monoid of the maintained value (S).
 * @tparam K Dimension.
 */
template<typename PT, typename M, u32 K = tuple_size<PT>::value>
struct KDTree
{

#define KDT_MIS make_index_sequence<K>{}
#define KDT_ARGS const PT& a, const PT& b, index_sequence<Is...>

    template <size_t... Is> 
    PT min_pt(KDT_ARGS) { return { min(get<Is>(a), get<Is>(b))... }; }
    template <size_t... Is> 
    PT max_pt(KDT_ARGS) { return { max(get<Is>(a), get<Is>(b))... }; }

    template <size_t... Is> 
    bool cle_pt_impl(KDT_ARGS) { return ((get<Is>(a) <= get<Is>(b)) && ...); }
    bool cle_pt(const PT& a, const PT& b){ return cle_pt_impl(a, b, KDT_MIS); }

    using S = typename M::S;

    struct Node
    {
        // Vector sp -> ep describes the hypercube.
        PT sp, ep;
        S val;
        Node(const PT &_sp, const PT &_tp, const S &s = M::un()): 
            sp(_sp), ep(_tp), val(s) { }
        Node() { }
    };

    u32 n;
    vec<Node> t;
    vec<u32> rnk;

    KDTree(): n() { }
    KDTree(u32 _n, const vec<PT> &p, const vec<S> &s)
        : n(_n), t(), rnk(n)
    {
        u32 m = 1;
        while (m < n) m += m;
        t.resize(m * 2);
        vec<u32> idx(n);
        for (u32 i = 0; i < n; i++) idx[i] = i;
        build_rec<0>(p, s, idx, 0, n, 1);
    }

    // Vector st -> ed describes a hypercube.
    S prod(const PT &st, const PT &ed) { return prod_rec(st, ed, 1); }
    void set(u32 x, const S &v) { set_rec(rnk[x], v, 0, n, 1); }

    template<u32 D>
    void build_rec(const vec<PT> &p, const vec<S> &s, 
        vec<u32> &idx, u32 l, u32 r, u32 x)
    {
        if (l + 1 == r) {
            u32 idn = idx[l];
            t[x] = {p[idn], p[idn], s[idn]};
            rnk[idn] = l;
            return ;
        }
        u32 mid = (l + r) >> 1;
        nth_element(idx.data() + l, idx.data() + mid, idx.data() + r, 
            [&p](u32 a, u32 b) { return get<D>(p[a]) < get<D>(p[b]); });
        build_rec<(D + 1) % K>(p, s, idx, l, mid, x * 2);
        build_rec<(D + 1) % K>(p, s, idx, mid, r, x * 2 + 1);
        t[x].sp = min_pt(t[x * 2].sp, t[x * 2 + 1].sp, KDT_MIS);
        t[x].ep = max_pt(t[x * 2].ep, t[x * 2 + 1].ep, KDT_MIS);
        t[x].val = M::op(t[x * 2].val, t[x * 2 + 1].val);
    }

    S prod_rec(const PT &st, const PT &ed, u32 x)
    {
        if (cle_pt(st, t[x].sp) && cle_pt(t[x].ep, ed)) return t[x].val;
        if (!cle_pt(t[x].sp, ed) || !cle_pt(st, t[x].ep)) return M::un();
        return M::op(prod_rec(st, ed, x * 2), prod_rec(st, ed, x * 2 + 1));
    }

    void set_rec(u32 p, const S &v, u32 l, u32 r, u32 x)
    {
        if (l + 1 == r) {
            t[x].val = v;
            return ;
        }
        u32 mid = (l + r) >> 1;
        if (p < mid) set_rec(p, v, l, mid, x * 2);
        else set_rec(p, v, mid, r, x * 2 + 1);
        t[x].val = M::op(t[x * 2].val, t[x * 2 + 1].val);
    }
#undef KDT_MIS
#undef KDT_ARGS
};

template<typename PT, typename M, u32 K = tuple_size<PT>::value>
struct KDForest
{
    using S = typename M::S;
    using KDT = KDTree<PT, M, K>;

    vec<KDT> f;
    
    KDForest(): f() { }
    
    void push_back(const PT &v, const S &w)
    {
        u32 ns = 1;
        vec<PT> p(1, v);
        vec<S> s(1, w);
        while (!f.empty() && f.back().n == ns) {
            p.resize(ns * 2), s.resize(ns * 2);
            auto &nw = f.back();
            for (u32 i = 0; i < ns; i++) {
                auto &d = nw.t[nw.rnk[i] + ns];
                p[ns * 2 - i - 1] = d.sp;
                s[ns * 2 - i - 1] = d.val;
            }
            ns *= 2;
            f.pop_back();
        }
        reverse(p.begin(), p.end());
        reverse(s.begin(), s.end());
        f.emplace_back(ns, p, s);
    }

    S prod(const PT &st, const PT &ed)
    {
        S res = M::un();
        for (auto &t: f) {
            res = M::op(res, t.prod(st, ed));
        }
        return res;
    }

    void set(u32 x, const S &s)
    {
        for (auto &t: f) {
            if (x < t.n) {
                t.set(x, s);
                break;
            }
            x -= t.n;
        }
    }
};

struct MonoidSum
{
    using S = u64;
    static S op(S a, S b) { return a + b; }
    static S un() { return 0; }
};

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    using Point = pair<u32, u32>;
    u32 n, q;
    cin >> n >> q;
    KDForest<Point, MonoidSum> t;
    for (u32 i = 0, x, y, z; i < n; i++) {
        cin >> x >> y >> z;
        t.push_back({x, y}, z);
    }
    u32 op, a, b, c, d;
    while (q--) {
        cin >> op >> a >> b >> c;
        if (op == 0) t.push_back({a, b}, c);
        else {
            cin >> d;
            cout << t.prod({a, b}, {c - 1, d - 1}) << '\n';
        }
    }
    return 0;
}