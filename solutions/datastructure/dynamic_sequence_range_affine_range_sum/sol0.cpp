#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <atcoder/modint>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

namespace dynamic_sequence
{
namespace dynamic_sequence_base
{
template<typename M>
class DynamicSequenceBase
{
protected:
    using MS = typename M::MS;
    using MA = typename M::MA;
    using S = typename MS::S;
    using A = typename MA::S;

    struct Node
    {
        S val, prd;
        A tag;
        bool rev;
        u32 ls, rs;
        u32 siz;
        Node(const S &s = MS::un(), u32 real = 1u): 
            val(s), prd(s), tag(MA::un()), rev(), 
            ls(), rs(), siz(real) { }
    };

    static vec<Node> t;
    static vec<u32> trash_bin;

    static void reserve(u32 s) { t.reserve(s); }
    static void reserve_trash_bin(u32 s) { trash_bin.reserve(s); }

    template<typename... Args>
    static u32 new_node(Args &&...args) 
    {
        if (trash_bin.size()) {
            u32 v = trash_bin.back();
            t[v] = S(std::forward<Args>(args)...);
            trash_bin.pop_back();
            return v;
        }
        t.emplace_back(std::forward<Args>(args)...);
        return t.size() - 1;
    }

    static void apply_at(u32 x, const A &a)
    {
        t[x].prd = M::act(t[x].prd, a, t[x].siz);
        t[x].val = M::act(t[x].val, a, 1u);
        t[x].tag = MA::op(t[x].tag, a);
    }

    static void toggle(u32 x)
    {
        t[x].rev ^= 1;
        swap(t[x].ls, t[x].rs);
        if constexpr (!MS::cm) t[x].prd = MS::ts(t[x].prd);
    }

    static void push_down(u32 x)
    {
        // if (t[x].tag == MA::un() && !t[x].rev) return ;
        u32 ls = t[x].ls, rs = t[x].rs;
        if (!(t[x].tag == MA::un())) {
            if (ls) apply_at(ls, t[x].tag);
            if (rs) apply_at(rs, t[x].tag);
            t[x].tag = MA::un();
        }
        if (t[x].rev) {
            t[x].rev = false;
            if (ls) toggle(ls);
            if (rs) toggle(rs);
        }
    }

    static void push_up(u32 x)
    {
        u32 ls = t[x].ls, rs = t[x].rs;
        t[x].siz = 1, t[x].prd = t[x].val;
        if (ls) {
            t[x].prd = MS::op(t[ls].prd, t[x].prd);
            t[x].siz += t[ls].siz;
        }
        if (rs) {
            t[x].prd = MS::op(t[x].prd, t[rs].prd);
            t[x].siz += t[rs].siz;
        }
    }

    static void split(u32 k, u32 x, u32 &l, u32 &r)
    {
        if (x == 0) {
            l = r = 0; /// !!!
            return ;
        }
        push_down(x);
        u32 sls = t[t[x].ls].siz;
        if (sls >= k) {
            split(k, t[r = x].ls, l, t[x].ls);
        }
        else {
            split(k - sls - 1, t[l = x].rs, t[x].rs, r);
        }
        push_up(x);
    }

    static u32 merge(u32 x, u32 y)
    {
        if (x == 0) return y;
        if (y == 0) return x;
        if (((rng() * (t[x].siz + t[y].siz)) >> 32) < t[x].siz) {
            push_down(x);
            t[x].rs = merge(t[x].rs, y);
            push_up(x);
            return x;
        }
        push_down(y);
        t[y].ls = merge(x, t[y].ls);
        push_up(y);
        return y;
    }

    template<typename F>
    static u32 build(u32 l, u32 r, F &&s)
    {
        if (l + 1 == r) return new_node(s(l));
        u32 mid = (l + r) / 2;
        u32 pmid = new_node(s(mid));
        if (l < mid) t[pmid].ls = build(l, mid, s);
        if (mid + 1 < r) t[pmid].rs = build(mid + 1, r, s);
        push_up(pmid);
        return pmid;
    }

    template<typename OutputIt>
    static OutputIt print(u32 x, OutputIt o)
    {
        push_down(x);
        if (t[x].ls) o = print(t[x].ls, o);
        *o++ = t[x].val;
        if (t[x].rs) o = print(t[x].rs, o);
        return o;
    }

    static u64 rng() {
        static u64 x_ = 88172645463325252ULL;
        return x_ ^= x_ << 7, x_ ^= x_ >> 9, x_ & 0xFFFFFFFFull;
    }

    static void discard(u32 x)
    {
        if (t[x].ls) discard(t[x].ls);
        if (t[x].rs) discard(t[x].rs);
        trash_bin.emplace_back(x);
    }

};

template<typename M> vec<typename DynamicSequenceBase<M>::Node> 
    DynamicSequenceBase<M>::t(1, {M::MS::un(), 0u});
template<typename M> vec<u32> DynamicSequenceBase<M>::trash_bin;

}


template<typename M> class DynamicSequence:
    dynamic_sequence_base::DynamicSequenceBase<M>
{
    u32 root;
    using Self = DynamicSequence<M>;
    using Base = dynamic_sequence_base::DynamicSequenceBase<M>;
    using typename Base::S;
    using typename Base::A;
    using Base::split;
    using Base::merge;

    DynamicSequence(u32 rt): root(rt) { }
public:

    using Base::reserve;
    using Base::reserve_trash_bin;

    DynamicSequence(): root(0) { }
    template<typename F>
    DynamicSequence(u32 n, F &&f): root(Base::template build(0, n, f)) { }
    template<typename T>
    DynamicSequence(const vec<T> &s): DynamicSequence(s.size(), 
        [&s](u32 x) { return s[x]; }) { }
    DynamicSequence(const Self &t)
    {
        if (t.empty()) {
            root = 0;
            return ;
        }
        vec<S> tmp(size());
        Base::print(t.root, tmp.begin());
        root = Base::build(0, tmp.size(), tmp);
    }
    DynamicSequence(Self &&t): root(t.root) { t.root = 0; }
    ~DynamicSequence() { if (!empty()) clear(); }

    Self& operator=(const Self& other) 
    {
        if (this != &other) {
            if (!empty()) clear();
            *this = Self(other);
        }
        return *this;
    }
    Self& operator=(Self && other)
    {
        if (this != &other) {
            if (!empty()) clear();
            root = other.root;
            other.root = 0;
        }
        return *this;
    }

    constexpr u32 size() { return Base::t[root].siz; }
    constexpr bool empty() { return root == 0; }
#define __forward std::forward<Args>(args)...
#define __new_node() Base::template new_node(__forward)
    template<typename... Args>
    void emplace(u32 x, Args &&...args)
    {
        if (root == 0) {
            root = __new_node();
        }
        else if (x == 0) {
            u32 mid = __new_node();
            root = merge(mid, root);
        }
        else if (x == size()) {
            u32 mid = __new_node();
            root = merge(root, mid);
        }
        else {
            u32 pre = 0, suf = 0;
            split(x, root, pre, suf);
            u32 mid = __new_node();
            mid = merge(pre, mid);
            root = merge(mid, suf);
        }
    }
#undef __new_node

    template<typename... Args>
    void emplace_back(Args &&...args) { emplace(size(), __forward); }
    template<typename... Args>
    void emplace_front(Args &&...args) { emplace(0, __forward); }
#undef __forward

    void push_back(const S &s) { emplace_back(s); }

    void splice(u32 x, Self &&other)
    {
        if (root == 0) {
            root = other.root;
        }
        else if (x == 0) {
            root = merge(other.root, root);
        }
        else if (x == size()) {
            root = merge(root, other.root);
        }
        else {
            u32 pre = 0, suf = 0;
            split(x, root, pre, suf);
            root = merge(pre, other.root);
            root = merge(root, suf);
        }
        other.root = 0;
    }

    void erase(u32 x)
    {
        u32 pre = 0, mid = 0, suf = 0;
        split(x + 1, root, mid, suf);
        split(x, mid, pre, mid);
        Base::trash_bin.emplace_back(mid);
        root = merge(pre, suf);
    }

    void erase(u32 l, u32 r)
    {
        u32 pre = 0, mid = 0, suf = 0;
        split(r, root, mid, suf);
        split(l, mid, pre, mid);
        Base::discard(mid);
        root = merge(pre, suf);
    }

    void clear()
    {
        Base::discard(root);
        root = 0;
    }

    void reverse(u32 l, u32 r)
    {
        u32 pre = 0, mid = 0, suf = 0;
        split(r, root, mid, suf);
        split(l, mid, pre, mid);
        Base::toggle(mid);
        mid = merge(pre, mid);
        root = merge(mid, suf);
    }

    void toggle_reverse() { Base::toggle(root); }

    Self extract(u32 l, u32 r)
    {
        u32 pre = 0, mid = 0, suf = 0;
        split(r, root, mid, suf);
        split(l, mid, pre, mid);
        Self res(mid);
        root = merge(pre, suf);
        return res;
    }

    void apply(u32 l, u32 r, const A &a)
    {
        u32 pre = 0, mid = 0, suf = 0;
        split(r, root, mid, suf);
        split(l, mid, pre, mid);
        Base::apply_at(mid, a);
        mid = merge(pre, mid);
        root = merge(mid, suf);
    }

    S prod(u32 l, u32 r)
    {
        u32 pre = 0, mid = 0, suf = 0;
        split(r, root, mid, suf);
        split(l, mid, pre, mid);
        S res = Base::t[mid].prd;
        mid = merge(pre, mid);
        root = merge(mid, suf);
        return res;
    }

    S prod_all() { return Base::t[root].prd; }

    S operator[](u32 x)
    {
        u32 pre = 0, mid = 0, suf = 0;
        split(x + 1, root, mid, suf);
        split(x, mid, pre, mid);
        S res = Base::t[mid].val;
        mid = merge(pre, mid);
        root = merge(mid, suf);
        return res;
    }

    template<typename OutputIt>
    OutputIt print(u32 l, u32 r, OutputIt o)
    {
        u32 pre = 0, mid = 0, suf = 0;
        split(r, root, mid, suf);
        split(l, mid, pre, mid);
        auto res = Base::template print<OutputIt>(mid, o);
        mid = merge(pre, mid);
        root = merge(mid, suf);
        return res;
    }
};
}
using dynamic_sequence::DynamicSequence;
using mint = atcoder::modint998244353;

struct MonoidLF
{
    using S = pair<mint, mint>;
    static S op(const S &a, const S &b) 
    {
        return S(a.first * b.first, a.second * b.first + b.second);
    }
    static S un() { return S(1, 0); }
};

struct MonoidAdd
{
    using S = mint;
    static S op(S a, S b) { return a + b; }
    static S un() { return 0; }
    static constexpr bool cm = true;
};

struct ActedMonoid
{
    using MA = MonoidLF;
    using MS = MonoidAdd;
    static mint act(mint v, const pair<mint, mint> & a, u32 len)
    {
        return a.first * v + a.second * len;
    }
};

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, q;
    cin >> n >> q;
    vec<mint> a(n);
    for (auto &i: a) {
        u32 x;
        cin >> x;
        i = x;
    }
    DynamicSequence<ActedMonoid>::reserve(n + q);
    DynamicSequence<ActedMonoid>::reserve_trash_bin(n + q);
    DynamicSequence<ActedMonoid> ds(a);
    u32 op, w, x, y, z;
    while (q--) {
        cin >> op;
        if (op == 0) {
            cin >> x >> y;
            ds.emplace(x, y);
        }
        else if (op == 1) {
            cin >> x;
            ds.erase(x);
        }
        else if (op == 2) {
            cin >> x >> y;
            ds.reverse(x, y);
        }
        else if (op == 3) {
            cin >> w >> x >> y >> z;
            ds.apply(w, x, {y, z});
        }
        else {
            cin >> x >> y;
            cout << ds.prod(x, y).val() << '\n';
        }
    }
    return 0;
}