#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <map>
#include <array>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

template<typename T, typename Dic = map<T, u32>>
struct SAM
{
    struct Node: Dic
    {
        u32 len, link;
        Node(u32 _len, u32 _link = 0): Dic(), len(_len), link(_link) { }
    };

    vec<Node> dat;

    SAM(): dat() { new_node(0); }
    SAM(T): dat() { new_node(0); }
    SAM(T, Dic): dat() { new_node(0); }

    template<typename ...Args>
    u32 new_node(Args &&...args) 
    {
        dat.emplace_back(std::forward<Args>(args)...);
        return dat.size() - 1;
    }

    void reserve(u32 s) 
    {
        if (dat.size() >= ++s) return ;
        dat.reserve(s);
    }

    u32 insert(u32 p, T ch)
    {
        if (dat[dat[p][ch]].len == dat[p].len + 1) return dat[p][ch];
        u32 cur = new_node(dat[p].len + 1), chk = dat[p][ch];
        while (p && !dat[p][ch]) {
            dat[p][ch] = cur;
            p = dat[p].link;
        }
        if (!p && !dat[0u][ch]) {
            dat[0u][ch] = cur;
            dat[cur].link = 0u;
            return cur;
        }
        u32 q = dat[p][ch];
        if (dat[p].len + 1 == dat[q].len) {
            dat[cur].link = q;
            return cur;
        }
        u32 cl = new_node(dat[q]);
        dat[q].link = (dat[cur].link = cl);
        dat[cl].len = dat[p].len + 1;
        while (dat[p][ch] == q) {
            dat[p][ch] = cl;
            p = dat[p].link;
        }
        return chk ? cl : cur;
    }

    const Node &operator[](u32 p) { return dat[p]; }
    u32 size() const { return dat.size(); }
};

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    string s;
    cin >> s;
    SAM<int, array<u32, 26>> atm;
    u32 las = 0;
    for (auto i: s) {
        las = atm.insert(las, i - 'a');
    }
    u64 ans = 0;
    for (u32 i = 1; i <= atm.size(); i++) {
        ans += atm[i].len - atm[atm[i].link].len;
    }
    cout << ans << endl;
    return 0;
}