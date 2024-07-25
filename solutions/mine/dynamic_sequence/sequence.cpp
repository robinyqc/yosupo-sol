#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <memory>
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

template<typename T, typename D, bool persistent> struct NodePtr;
template<typename T, typename D> struct NodePtr<T, D, true> 
{ 
    struct MyShared
    {
        T* t;
        MyShared(): t(nullptr) { }
        MyShared(nullptr_t nul): t(nul) { }
        template<typename Alloc>
        MyShared(T *s, Alloc, Alloc): t(s) { }
        constexpr operator bool() const { return t; }
        constexpr T *operator->() const { return t; }
        constexpr T &operator*() const { return *t; }
        constexpr void reset() { t = nullptr; }
    };
    // using type = MyShared;
    using type = shared_ptr<T>;
    template<typename Alloc>
    static constexpr type make_ptr(T *x, Alloc __alloc)
    {
        return type(x, __alloc, __alloc);
    }
};
template<typename T, typename D> struct NodePtr<T, D, false> 
{ 
    using type = unique_ptr<T, D>; 
    template<typename Alloc>
    static constexpr type make_ptr(T *x, Alloc __alloc)
    {
        return type(x, __alloc);
    }
};

template<typename M, typename Alloc, bool persistent>
class DynamicSequenceBase
{
protected:

    using MS = typename M::MS;
    // [[ds_act]]
    using MA = typename M::MA;
    using S = typename MS::S;
    // [[ds_act]]
    using A = typename MA::S;

    struct Node;

    template<typename T>
    class AllocatorWithDeleter: 
        public allocator_traits<Alloc>::template rebind_alloc<T>
    {
        using Base = typename allocator_traits<Alloc>::template rebind_alloc<T>;
        using awd_traits = allocator_traits<AllocatorWithDeleter>;
    public:
        using Base::Base;
        // template<typename U> using rebind = AllocatorWithDeleter<U>;
        void operator()(Node *ptr)
        {
            awd_traits::destroy(*this, ptr);
            awd_traits::deallocate(*this, ptr, 1);
        }
    };

    AllocatorWithDeleter<Node> __alloc;
    using alloc_traits = allocator_traits<AllocatorWithDeleter<Node>>;

    using PtrImpl = NodePtr<Node, AllocatorWithDeleter<Node>, persistent>;
    using Ptr = typename PtrImpl::type;

    struct Node
    {
        S val;
        // [[ds_prd]]
        S prd;
        // [[ds_act]]
        A tag;
        // [[ds_rev]]
        bool rev;
        Ptr ls, rs;
        u32 siz;
        Node(const S &s = MS::un(), u32 real = 1u): 
            val(s), 
            // [[ds_prd]]
            prd(s), 
            // [[ds_act]]
            tag(MA::un()), 
            // [[ds_rev]]
            rev(), 
            ls(), rs(nullptr), siz(real) { }
        ~Node() { /* cerr << "[DEBUG] A node has been destroyed." << endl; */ }
    };

    DynamicSequenceBase(): __alloc() { }

    template<typename... Args>
    Ptr new_node(Args &&...args) 
    {
        Node *res(alloc_traits::allocate(__alloc, 1u));
        alloc_traits::construct(__alloc, res, std::forward<Args>(args)...);
        return PtrImpl::make_ptr(res, __alloc);
    }

    void del_node(Ptr &x) { x.reset(); }

    // [[ds_act]]
    void apply_at(Ptr &x, const A &a)
    {
        // [[ds_prd]]
        x->prd = M::act(x->prd, a, x->siz);
        x->val = M::act(x->val, a, 1u);
        x->tag = MA::op(x->tag, a);
    }

    // [[ds_rev]]
    void toggle(Ptr &x)
    {
        x->rev ^= 1;
        swap(x->ls, x->rs);
        if constexpr (!MS::cm) x->prd = MS::ts(x->prd);
    }

    // [[ds_range_opt]]
    void push_down(Ptr &x)
    {
        if constexpr (persistent) {
            if (x->ls) x->ls = new_node(*x->ls);
            if (x->rs) x->rs = new_node(*x->rs);
        }
        // [[ds_act]]
        if (!(x->tag == MA::un())) {
            // Maybe inline function will be faster.
            if (x->ls) apply_at(x->ls, x->tag);
            if (x->rs) apply_at(x->rs, x->tag);
            x->tag = MA::un();
        }
        // [[ds_rev]]
        if (x->rev) {
            x->rev = false;
            if (x->ls) toggle(x->ls);
            if (x->rs) toggle(x->rs);
        }
    }

    void push_up(Ptr &x)
    {
        x->siz = 1;
        // [[ds_prd]]
        x->prd = x->val;
        if (x->ls) {
            // [[ds_prd]]
            x->prd = MS::op(x->ls->prd, x->prd);
            x->siz += x->ls->siz;
        }
        if (x->rs) {
            // [[ds_prd]]
            x->prd = MS::op(x->prd, x->rs->prd);
            x->siz += x->rs->siz;
        }
    }

    void split(Ptr &&x, u32 k, Ptr &l, Ptr &r)
    {
        if (!x) {
            l.reset();
            r.reset();
            return ;
        }

        // [[ds_range_opt]]
        push_down(x);

        u32 sls = x->ls ? x->ls->siz : 0u;
        if (sls >= k) {
            r = std::move(x);
            // [[ds_no_range_opt]]
            // if (r->ls) r->ls = new_node(*(r->ls));
            split(std::move(r->ls), k, l, r->ls);
            push_up(r);
        }
        else {
            l = std::move(x);
            // [[ds_no_range_opt]]
            // if (l->rs) l->rs = new_node(*(l->rs));
            split(std::move(l->rs), k - sls - 1, l->rs, r);
            push_up(l);
        }
    }

    Ptr merge(Ptr &&x, Ptr &&y)
    {
        if (!x) return y;
        if (!y) return x;
        if (((rng() * (x->siz + y->siz)) >> 32) < x->siz) {
            // [[ds_range_opt]]
            push_down(x);
            x->rs = merge(std::move(x->rs), std::forward<Ptr>(y));
            push_up(x);
            return x;
        }
        // [[ds_range_opt]]
        push_down(y);
        y->ls = merge(std::forward<Ptr>(x), std::move(y->ls));
        push_up(y);
        return y;
    }

    Ptr *search(Ptr &&x, u32 k)
    {
        ++k;
        Ptr *p = &x;
        u32 lsiz;
        while (k) {
            push_down(*p);
            lsiz = (*p)->ls ? (*p)->ls->siz : 0;
            if (lsiz >= k) p = &((*p)->ls);
            else if (lsiz + 1 < k) p = &((*p)->rs), k -= lsiz + 1;
            else {
                return p;
            }
        }
        return p;
    }

    // max r that f(prod([0, r))) == false
    // [[ds_prd]]
    template<typename F>
    u32 max_right(Ptr &&x, F &&f) 
    {
        Ptr *p = &x;
        u32 res = 0;
        S lval = MS::un();
        while (*p) {
            // [[ds_range_opt]]
            push_down(*p);
            if ((*p)->ls && f(M::op(lval, (*p)->ls->val))) p = &((*p)->ls);
            else {
                if ((*p)->ls) {
                    lval = M::op(lval, (*p)->ls->val);
                    res += (*p)->ls->siz;
                }
                if (f(M::op(lval, (*p)->val))) return res;
                lval = M::op(lval, (*p)->val);
                res++;
                p = &((*p)->rs);
            }
        }
        return res;
    }

    // min p that f(val(p)) == false
    template<typename F>
    u32 upper_bound(Ptr &&x, F &&f) 
    {
        Ptr *p = &x;
        u32 res = 0;
        while (*p) {
            // [[ds_range_opt]]
            push_down(*p);
            if (f((*p)->val)) {
                if ((*p)->ls) {
                    res += (*p)->ls->siz;
                }
                res += 1;
                p = &((*p)->rs);
            }
            else {
                p = &((*p)->ls);
            }
        }
        return res;
    }

    template<typename F>
    Ptr build(u32 l, u32 r, F &&s)
    {
        if (l + 1 == r) return new_node(s(l));
        u32 mid = (l + r) / 2;
        Ptr pmid = new_node(s(mid));
        if (l < mid) pmid->ls = build(l, mid, s);
        if (mid + 1 < r) pmid->rs = build(mid + 1, r, s);
        push_up(pmid);
        return pmid;
    }

    template<typename OutputIt>
    OutputIt print(Ptr &x, OutputIt o)
    {
        // [[ds_range_opt]]
        push_down(x);
        if (x->ls) o = print(x->ls, o);
        *o++ = x->val;
        if (x->rs) o = print(x->rs, o);
        return o;
    }

    static u64 rng() {
        static u64 x_ = 88172645463325252ULL;
        return x_ ^= x_ << 7, x_ ^= x_ >> 9, x_ & 0xFFFFFFFFull;
    }

};

}


template<typename M, typename Alloc = std::allocator<int>> 
    class DynamicSequence:
        dynamic_sequence_base::DynamicSequenceBase<M, Alloc, false>
{
    using Self = DynamicSequence;
    using Base = dynamic_sequence_base::DynamicSequenceBase<M, Alloc, false>;
    using typename Base::MS;
    using typename Base::S;
    // [[ds_act]]
    using typename Base::A;
    using typename Base::Ptr;
    using Base::split;
    using Base::merge;

    Ptr root;
    DynamicSequence(Ptr &&rt): root(rt) { }
public:

    DynamicSequence(): root() { }
    template<typename F>
    DynamicSequence(u32 n, F &&f): root(Base::template build(0, n, f)) { }
    DynamicSequence(const vec<S> &s): 
        Self(s.size(), [&s](u32 x) { return s[x]; }) { }
    DynamicSequence(const Self &t)
    {
        if (t.empty()) {
            root.reset();
            return ;
        }
        vec<S> tmp(t.size());
        t.print(tmp.begin());
        root = Base::template build(0, tmp.size(), tmp);
    }
    DynamicSequence(Self &&t): root(std::move(t.root)) { }
    ~DynamicSequence() { if (!empty()) clear(); }

    Self& operator=(const Self& other) 
    {
        if (this != &other) {
            if (!empty()) clear();
            *this = Self(other);
        }
        return *this;
    }
    Self& operator=(Self &&other)
    {
        if (this != &other) {
            if (!empty()) clear();
            root = std::move(other.root);
        }
        return *this;
    }

    constexpr u32 size() const { return root ? root->siz : 0u; }
    constexpr bool empty() const { return !root; }
#define __ds_forward std::forward<Args>(args)...
#define __ds_new_node() Base::template new_node(__ds_forward)
    template<typename... Args>
    void emplace(u32 x, Args &&...args)
    {
        if (!root) {
            root = __ds_new_node();
        }
        else if (x == 0) {
            root = merge(__ds_new_node(), std::move(root));
        }
        else if (x == size()) {
            root = merge(std::move(root), __ds_new_node());
        }
        else {
            Ptr pre, suf;
            split(std::move(root), x, pre, suf);
            root = merge(merge(std::move(pre), __ds_new_node()), 
                         std::move(suf));
        }
    }
#undef __ds_new_node

    template<typename... Args>
    void emplace_back(Args &&...args) { emplace(size(), __ds_forward); }
    template<typename... Args>
    void emplace_front(Args &&...args) { emplace(0, __ds_forward); }
#undef __ds_forward

    void push_back(const S &s) { emplace_back(s); }
    void push_back(S &&s) { emplace_back(s); }
    void push_front(const S &s) { emplace_front(s); }
    void push_front(S &&s) { emplace_front(s); }

    void splice(u32 x, Self &&other)
    {
        if (other.empty()) {
            return ;
        }
        if (!root) {
            root = std::move(other.root);
        }
        else if (x == 0) {
            root = merge(std::move(other.root), std::move(root));
        }
        else if (x == size()) {
            root = merge(std::move(root), std::move(other.root));
        }
        else {
            Ptr pre, suf;
            split(std::move(root), x, pre, suf);
            root = merge(merge(std::move(pre), std::move(other.root)), 
                         std::move(suf));
        }
    }
    void splice(u32 x, Self &other) { splice(x, std::move(other)); }

    void erase(u32 x) { erase(x, x + 1); }

    void erase(u32 l, u32 r)
    {
        if (l == r) return ;
        Ptr pre, mid, suf;
        split(std::move(root), r, mid, suf);
        split(std::move(mid), l, pre, mid);
        Base::del_node(mid);
        root = merge(std::move(pre), std::move(suf));
    }

    void clear() { Base::del_node(root); }

    // [[ds_rev]]
    void reverse(u32 l, u32 r)
    {
        if (l == r) return ;
        Ptr pre, mid, suf;
        split(std::move(root), r, mid, suf);
        split(std::move(mid), l, pre, mid);
        Base::toggle(mid);
        root = merge(merge(std::move(pre), std::move(mid)), std::move(suf));
    }
    // [[ds_rev]]
    void toggle_reverse() 
    {
        if (empty()) return ; 
        Base::toggle(root); 
    }

    Self extract(u32 l, u32 r)
    {
        if (l == r) return Self();
        Ptr pre, mid, suf;
        split(std::move(root), r, mid, suf);
        split(std::move(mid), l, pre, mid);
        Self res(std::move(mid));
        root = merge(std::move(pre), std::move(suf));
        return res;
    }
    // [[ds_act]]
    void apply(u32 l, u32 r, const A &a)
    {
        if (l == r) return ;
        Ptr pre, mid, suf;
        split(std::move(root), r, mid, suf);
        split(std::move(mid), l, pre, mid);
        Base::apply_at(mid, a);
        root = merge(merge(std::move(pre), std::move(mid)), std::move(suf));
    }

    void set(u32 x, const S &s)
    {
        Ptr pre, mid, suf;
        split(std::move(root), x + 1, mid, suf);
        split(std::move(mid), x, pre, mid);
        mid->val = s;
        root = merge(merge(std::move(pre), std::move(mid)), std::move(suf));
    }
    // [[ds_prd]]
    S prod(u32 l, u32 r)
    {
        if (l == r) return MS::un();
        Ptr pre, mid, suf;
        split(std::move(root), r, mid, suf);
        split(std::move(mid), l, pre, mid);
        S res = mid->prd;
        root = merge(merge(std::move(pre), std::move(mid)), std::move(suf));
        return res;
    }
    // [[ds_prd]]
    S prod_all() const
    { 
        if (empty()) return MS::un();
        return root->prd; 
    }
    // [[ds_prd]]
    template<typename F>
    u32 max_right(F &&f)
    {
        if (empty()) return 0;
        return Base::template max_right(std::move(root), f);
    }
    template<typename F>
    u32 upper_bound(F &&f)
    {
        if (empty()) return 0;
        return Base::template upper_bound(std::move(root), f);
    }

    S at(u32 x)
    {
        return (*Base::search(std::move(root), x))->val;
    }

    template<typename OutputIt>
    OutputIt print(u32 l, u32 r, OutputIt o)
    {
        if (l == r) return o;
        Ptr pre, mid, suf;
        split(std::move(root), r, mid, suf);
        split(std::move(mid), l, pre, mid);
        auto res = Base::template print<OutputIt>(mid, o);
        root = merge(merge(std::move(pre), std::move(mid)), std::move(suf));
        return res;
    }

    template<typename OutputIt>
    OutputIt print(OutputIt o)
    {
        if (empty()) return o;
        return Base::template print<OutputIt>(root, o);
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