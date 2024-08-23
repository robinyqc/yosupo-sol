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

template<typename T, int d>
struct BinTrie
{
    struct Node
    {
        u32 siz;
        Node *ch[2];
        Node(): siz(), ch{nullptr, nullptr} { }
    };

    Node *root;

    BinTrie(): root(new Node()) { }

    ~BinTrie() { del(root); }

    void insert(T x)
    {
        Node *ptr = root;
        for (int i = d - 1; i >= 0; i--) {
            int w = (x >> i) & 1;
            ptr->siz++;
            if (!ptr->ch[w]) ptr->ch[w] = new Node();
            ptr = ptr->ch[w];
        }
        ptr->siz++;
    }

    u32 size() { return root->siz; }

    u32 count(T x)
    {
        Node *ptr = root;
        for (int i = d - 1; i >= 0; i--) {
            int w = (x >> i) & 1;
            if (!ptr->ch[w]) return 0u;
            ptr = ptr->ch[w];
        }
        return ptr->siz;
    }

    void erase(T x)
    {
        Node *ptr = root;
        for (int i = d - 1; i >= 0; i--) {
            int w = (x >> i) & 1;
            ptr->siz--;
            ptr = ptr->ch[w];
        }
        ptr->siz--;
    }

    void del(Node *x)
    {
        if (x->ch[0]) del(x->ch[0]);
        if (x->ch[1]) del(x->ch[1]);
        delete x;
    }

    T closest(T x) 
    {
        T res = static_cast<T>(0);
        Node *ptr = root;
        for (int i = d - 1; i >= 0; i--) {
            int w = (x >> i) & 1;
            if (ptr->ch[w] && ptr->ch[w]->siz) {
                ptr = ptr->ch[w], res = res * 2 + w;
            }
            else ptr = ptr->ch[1 - w], res = res * 2 + 1 - w;
        }
        return res;
    }

    T furthest(T x)
    {
        T res = static_cast<T>(0);
        Node *ptr = root;
        for (int i = d - 1; i >= 0; i--) {
            int w = (x >> i) & 1;
            if (ptr->ch[1 - w] && ptr->ch[1 - w]->siz) {
                ptr = ptr->ch[1 - w], res = res * 2 + 1 - w;
            }
            else ptr = ptr->ch[w], res = res * 2 + w;
        }
        return res;
    }
};

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 q;
    cin >> q;
    BinTrie<u32, 30> bt;
    u32 op, x;
    while (q--) {
        cin >> op >> x;
        if (op == 0) {
            if (!bt.count(x)) bt.insert(x);
        }
        else if (op == 1) {
            if (bt.count(x)) bt.erase(x);
        }
        else {
            cout << (bt.closest(x) ^ x) << '\n';
        }
    }
    return 0;
}