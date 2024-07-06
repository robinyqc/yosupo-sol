#include <set>
#include <vector>
#include <memory>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <atcoder/modint>
#include <atcoder/segtree>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

using mint = atcoder::modint998244353;
template<typename T> using vec = vector<T>;

struct lf
{
    mint k, b; // k * x + b
    lf(mint _k = 1, mint _b = 0): k(_k), b(_b) { }
    mint operator()(mint x) const { return k * x + b; }
    lf operator*(const lf &t) const { return lf(k * t.k, b * t.k + t.b); }
};
lf lf_unit() { return lf(); }
lf lf_op(const lf& a, const lf& b) { return a * b; }

struct Monoid
{
    using S = pair<lf, lf>;
    static S op(const S &x, const S &y) 
    { 
        return S(x.first * y.first, y.second * x.second); 
    }
    static S un() { return S(lf(), lf()); }
    static S ts(S x) { return S(x.second, x.first); }
};

template<typename M, u32 dep = 30>
class DynamicSegmentTree
{
    using Self = DynamicSegmentTree;
public:
    using S = typename M::S;
    DynamicSegmentTree(): root(nullptr) { }
    DynamicSegmentTree(u32 x, S v): root(nullptr)
    {
        auto *p = &root;
        for (u32 d = dep - 1; ~d; d--) {
            *p = new Node(v, 1);
            if ((x >> d) & 1) p = &((*p)->rs);
            else p = &((*p)->ls);
        }
        *p = new Node(v, 1);
        // cerr << "!!!\n";
    }
    DynamicSegmentTree(Self &&v): root(nullptr) { swap(root, v.root); }
    Self &operator=(Self &&v) { swap(root, v.root); return *this; }
    ~DynamicSegmentTree() { /*clog << "del: " << u64(root) << '\n';*/ delete root; }

    S prod_all() { return root == nullptr ? S(): root->val; }
    void merge(Self &b) 
    { 
        root = merge_rec(root, b.root); 
        b.root = nullptr;
    }
    Self split(u32 x) 
    {
        Self res;
        res.root = new Node();
        split_rec(x, root, res.root);
        return res;
    }
    u32 size() { return root == nullptr ? 0 : root->siz; }
    S prod(u32 l, u32 r) 
    {
        assert(l < size() && r <= size());
        if (l >= r) return M::un();
        Self ger = split(r);
        Self tar = split(l);
        S res = tar.prod_all();
        merge(tar);
        merge(ger);
    }
    void toggle_reverse() 
    {
        if (root != nullptr) root->put_tag();
    }

public:
    struct Node
    {
        using ptr = Node*;
        S val;
        u32 siz;
        bool rev;
        ptr ls, rs;
        Node(S v = M::un(), u32 s = 0): val(v), siz(s), rev(), ls(nullptr), rs(nullptr) { }
        ~Node()
        {
            delete ls;
            delete rs;
        }

        void push_up() 
        { 
            if (ls == nullptr) val = rs->val, siz = rs->siz;
            else if (rs == nullptr) val = ls->val, siz = ls->siz;
            else {
                val = M::op(ls->val, rs->val); 
                siz = ls->siz + rs->siz;
            }
        }

        void put_tag() { rev ^= 1, val = M::ts(val); }

        void push_down()
        {
            if (rev) {
                swap(ls, rs);
                if (ls != nullptr) ls->put_tag();
                if (rs != nullptr) rs->put_tag();
                rev = 0;
            }
        }
    };

    using ptr = Node*;
    ptr root;

    void split_rec(u32 x, ptr p, ptr &q)
    {
        p->push_down();
        u32 lsiz = p->ls == nullptr ? 0 : p->ls->siz;
        if (lsiz < x) split_rec(x - lsiz, p->rs, q->rs = new Node());
        else {
            swap(p->rs, q->rs);
            // 保证了不会到最后一层 orz
            if (lsiz != x) split_rec(x, p->ls, q->ls = new Node());
        }
        p->push_up();
        q->push_up();
    }

    ptr merge_rec(ptr p, ptr q)
    {
        // 保证了不会到最后一层
        if (p == nullptr) return q;
        if (q == nullptr) return p;
        p->push_down();
        q->push_down();
        p->ls = merge_rec(p->ls, q->ls);
        p->rs = merge_rec(p->rs, q->rs);
        q->ls = q->rs = nullptr;
        delete q;
        p->push_up();
        return p;
    }
};

using dtree = DynamicSegmentTree<Monoid>;

signed main() 
{
    ios::sync_with_stdio(false); 
    cin.tie(0), cout.tie(0);

    u32 n, q;
    cin >> n >> q;
    vec<int> p(n);
    vec<lf> fun(n);
    vec<dtree> t;
    vec<bool> dec(n);
    set<u32> odt;

    for (u32 i = 0, a, b; i < n; i++) {
        cin >> p[i] >> a >> b;
        fun[i] = {a, b};
        t.emplace_back(p[i], pair{fun[i], fun[i]});
        odt.emplace_hint(odt.end(), i);
    }
    odt.emplace_hint(odt.end(), n);
    atcoder::segtree<lf, lf_op, lf_unit> tall(fun);

    auto odt_split = [&odt, &t, &tall, &dec](u32 x)
    {
        auto it = odt.lower_bound(x);
        if (*it == x) return it;
        auto l = *--it;
        odt.erase(it);
        t[x] = t[l].split(x - l);
        tall.set(l, t[l].prod_all().first);
        tall.set(x, t[x].prod_all().first);
        dec[x] = dec[l];
        return odt.emplace_hint(odt.emplace(l).first, x);
    };

    auto odt_merge = [&odt, &t, &dec, &tall](u32 l, u32 r)
    {
        auto li = odt.lower_bound(l), ri = odt.lower_bound(r);
        auto &v = t[l];
        bool stat = dec[l];
        for (auto i = next(li); i != ri; ) {
            if (dec[*i] != stat) {
                t[*i].toggle_reverse();  
            }
            v.merge(t[*i]);
            tall.set(*i, lf());
            // assert(t[*i].root == nullptr);
            odt.erase(i++);
        }
    };

    for (u32 id = 0, op, i, x, y, z; id < q; id++) {
        cin >> op;
        if (op == 0) {
            cin >> i >> x >> y >> z;
            odt_split(i);
            odt_split(i + 1);
            t[i] = dtree(x, {lf(y, z), lf(y, z)});
            tall.set(i, lf(y, z));
            dec[i] = false;
        }
        else if (op == 1) {
            cin >> x >> y >> z;
            odt_split(x);
            odt_split(y);
            cout << tall.prod(x, y)(z).val() << '\n';
        }
        else {
            cin >> x >> y;
            odt_split(x), odt_split(y);
            odt_merge(x, y);
            if (dec[x] != op - 2) {
                t[x].toggle_reverse();
                dec[x] = !dec[x];
            }
            tall.set(x, t[x].prod_all().first);
        }
    }
    return 0;
}