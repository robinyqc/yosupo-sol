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

unsigned count_destroy;

namespace dynamic_sequence
{
namespace dynamic_sequence_base
{

template<typename T, typename D, bool persistent> struct NodePtr;
template<typename T, typename D> struct NodePtr<T, D, true> 
{ 
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
    using MA = typename M::MA;
    using S = typename MS::S;
    using A = typename MA::S;

    struct Node;

    template<typename T>
    class AllocatorWithDeleter: 
        public allocator_traits<Alloc>::template rebind_alloc<T>
    {
        using awd_traits = allocator_traits<AllocatorWithDeleter>;
        using Base = typename allocator_traits<Alloc>::template rebind_alloc<T>;
    public:
        using Base::Base;
        template<typename U> using rebind = AllocatorWithDeleter<U>;
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
        S val, prd;
        A tag;
        bool rev;
        Ptr ls, rs;
        u32 siz;
        Node(const S &s = MS::un(), u32 real = 1u): 
            val(s), prd(s), tag(MA::un()), rev(), 
            ls(), rs(nullptr), siz(real) { }
        ~Node() { count_destroy++; /* cerr << "[DEBUG] A node has been destroyed." << endl; */ }
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

    void apply_at(Ptr &x, const A &a)
    {
        x->prd = M::act(x->prd, a, x->siz);
        x->val = M::act(x->val, a, 1u);
        x->tag = MA::op(x->tag, a);
    }

    void toggle(Ptr &x)
    {
        x->rev ^= 1;
        swap(x->ls, x->rs);
        if constexpr (!MS::cm) x->prd = MS::ts(x->prd);
    }

    void push_down(Ptr &x)
    {
        if constexpr (persistent) {
            if (x->ls) x->ls = new_node(*x->ls);
            if (x->rs) x->rs = new_node(*x->rs);
        }
        if (!(x->tag == MA::un())) {
            // Maybe inline function will be faster.
            if (x->ls) apply_at(x->ls, x->tag);
            if (x->rs) apply_at(x->rs, x->tag);
            x->tag = MA::un();
        }
        if (x->rev) {
            x->rev = false;
            if (x->ls) toggle(x->ls);
            if (x->rs) toggle(x->rs);
        }
    }

    void push_up(Ptr &x)
    {
        x->siz = 1, x->prd = x->val;
        if (x->ls) {
            x->prd = MS::op(x->ls->prd, x->prd);
            x->siz += x->ls->siz;
        }
        if (x->rs) {
            x->prd = MS::op(x->prd, x->rs->prd);
            x->siz += x->rs->siz;
        }
    }

    void split(Ptr &&x, u32 k, Ptr &l, Ptr &r)
    {
        if (!x) {
            l.reset(); /// !!!
            r.reset();
            return ;
        }
        push_down(x);

        u32 sls = x->ls ? x->ls->siz : 0u;
        if (sls >= k) {
            r = x;
            split(std::move(r->ls), k, l, r->ls);
            push_up(r);
        }
        else {
            l = x;
            split(std::move(l->rs), k - sls - 1, l->rs, r);
            push_up(l);
        }
    }

    Ptr merge(Ptr &&x, Ptr &&y)
    {
        if (!x) return y;
        if (!y) return x;
        if (((rng() * (x->siz + y->siz)) >> 32) < x->siz) {
            push_down(x);
            x->rs = merge(std::move(x->rs), std::forward<Ptr>(y));
            push_up(x);
            return x;
        }
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
            else if (lsiz + 1 < k) p = &((*p)->rs), k -= lsiz - 1;
            else {
                return p;
            }
        }
        return p;
    }

    // max r that f([0, r)) == false
    template<typename F>
    u32 max_right(Ptr &&x, F &&f) 
    {
        Ptr *p = &x;
        u32 res = 0;
        S lval = MS::un();
        while (p) {
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


template<typename M> class PersistentDynamicSequence:
    dynamic_sequence_base::DynamicSequenceBase<M, std::allocator<int>, true>
{
    using Self = PersistentDynamicSequence<M>;
    using Base = dynamic_sequence_base::
        DynamicSequenceBase<M, std::allocator<int>, true>;
    using typename Base::S;
    using typename Base::A;
    using typename Base::Ptr;
    using Base::split;
    using Base::merge;

    Ptr root;
    PersistentDynamicSequence(const Ptr &rt): root(rt) { }
    PersistentDynamicSequence(Ptr &&rt): root(rt) { }

#define copy(x) (Base::new_node(*(x)))

public:

    PersistentDynamicSequence(): root() { }
    template<typename F>
    PersistentDynamicSequence(u32 n, F &&f): 
        root(Base::template build(0, n, f)) { }
    PersistentDynamicSequence(const vec<S> &s): 
        Self(s.size(), [&s](u32 x) { return s[x]; }) { }
    PersistentDynamicSequence(const Self &t): root(t.root) { }
    PersistentDynamicSequence(Self &&t): root(std::move(t.root)) { }
    ~PersistentDynamicSequence() { if (!empty()) clear(); }

    // Create a new tree of the sequence in O(t.size()) time
    static Self clone(const Self &t)
    {
        if (t.empty()) return Self();
        vec<S> tmp(t.size());
        t.print(tmp.begin());
        return Self(tmp);
    }

    Self& operator=(const Self& other) 
    {
        if (this != &other) {
            if (!empty()) clear();
            root = other.root;
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

    constexpr u32 size() { return root ? root->siz : 0u; }
    constexpr bool empty() { return !root; }
#define __ds_forward std::forward<Args>(args)...
#define __ds_new_node() Base::template new_node(__ds_forward)
    template<typename... Args>
    Self emplace(u32 x, Args &&...args)
    {
        if (!root) {
            return __ds_new_node();
        }
        if (x == 0) {
            return merge(__ds_new_node(), copy(root));
        }
        if (x == size()) {
            return merge(copy(root), __ds_new_node());
        }
        Ptr pre, suf;
        split(copy(root), x, pre, suf);
        return merge(merge(std::move(pre), __ds_new_node()), 
                     std::move(suf));
    }
#undef __ds_new_node

    template<typename... Args>
    Self emplace_back(Args &&...args) { return emplace(size(), __ds_forward); }
    template<typename... Args>
    Self emplace_front(Args &&...args) { return emplace(0, __ds_forward); }
#undef __ds_forward

    Self push_back(const S &s) { return emplace_back(s); }
    Self push_back(S &&s) { return emplace_back(s); }
    Self push_front(const S &s) { return emplace_front(s); }
    Self push_front(S &&s) { return emplace_front(s); }

    Self splice(u32 x, const Self &other)
    {
        if (!root) {
            return other.root;
        }
        else if (x == 0) {
            return merge(copy(other.root), copy(root));
        }
        else if (x == size()) {
            return merge(copy(root), copy(other.root));
        }
        Ptr pre, suf;
        split(copy(root), x, pre, suf);
        return merge(merge(std::move(pre), copy(other.root)), 
                     std::move(suf));
    }

    Self erase(u32 x) { return erase(x, x + 1); }

    Self erase(u32 l, u32 r)
    {
        Ptr pre, mid, suf;
        split(copy(root), r, mid, suf);
        split(std::move(mid), l, pre, mid);
        Base::del_node(mid);
        return merge(std::move(pre), std::move(suf));
    }

    void clear() { Base::del_node(root); }

    Self reverse(u32 l, u32 r)
    {
        Ptr pre, mid, suf;
        split(copy(root), r, mid, suf);
        split(std::move(mid), l, pre, mid);
        Base::toggle(mid);
        return merge(merge(std::move(pre), std::move(mid)), std::move(suf));
    }

    // O(1)
    Self toggle_reverse() 
    {
        Ptr nv(copy(root));
        Base::toggle(nv);
        return nv;
    }

    // extract [l, r) from the source sequence
    // first: [0, l) + [r, size())
    // second: [l, r)
    pair<Self, Self> extract(u32 l, u32 r)
    {
        Ptr pre, mid, suf;
        split(copy(root), r, mid, suf);
        split(std::move(mid), l, pre, mid);
        return {merge(std::move(pre), std::move(suf)), std::move(mid)};
    }

    Self apply(u32 l, u32 r, const A &a)
    {
        Ptr pre, mid, suf;
        split(copy(root), r, mid, suf);
        split(std::move(mid), l, pre, mid);
        Base::apply_at(mid, a);
        return merge(merge(std::move(pre), std::move(mid)), std::move(suf));
    }

    Self set(u32 x, const S &s)
    {
        Ptr pre, mid, suf;
        split(copy(root), x + 1, mid, suf);
        split(std::move(mid), x, pre, mid);
        mid->val = s;
        return merge(merge(std::move(pre), std::move(mid)), std::move(suf));
    }

    S prod(u32 l, u32 r)
    {
        Ptr pre, mid, suf;
        split(copy(root), r, mid, suf);
        split(std::move(mid), l, pre, mid);
        return mid->prd;
    }

    S prod_all() { return root->prd; }

    template<typename F>
    u32 max_right(F &&f)
    {
        return Base::template max_right(copy(root), f);
    }

    S at(u32 x)
    {
        return (*Base::search(copy(root), x))->val;
    }

    template<typename OutputIt>
    OutputIt print(u32 l, u32 r, OutputIt o)
    {
        Ptr pre, mid, suf;
        split(copy(root), r, mid, suf);
        split(std::move(mid), l, pre, mid);
        return Base::template print<OutputIt>(mid, o);
    }

    template<typename OutputIt>
    OutputIt print(OutputIt o)
    {
        return Base::template print<OutputIt>(copy(root), o);
    }
};
}
using dynamic_sequence::PersistentDynamicSequence;
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
    {
        PersistentDynamicSequence<ActedMonoid> ds(a);
        u32 op, w, x, y, z;
        while (q--) {
            cin >> op;
            if (op == 0) {
                cin >> x >> y;
                ds = ds.emplace(x, y);
            }
            else if (op == 1) {
                cin >> x;
                ds = ds.erase(x);
            }
            else if (op == 2) {
                cin >> x >> y;
                ds = ds.reverse(x, y);
            }
            else if (op == 3) {
                cin >> w >> x >> y >> z;
                ds = ds.apply(w, x, {y, z});
            }
            else {
                cin >> x >> y;
                cout << ds.prod(x, y).val() << '\n';
            }
        }
    }
    cerr << count_destroy << endl;
    return 0;
}