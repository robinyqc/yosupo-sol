#include <cmath>
#include <tuple>
#include <vector>
#include <utility>
#include <cstring>
#include <iostream>
#include <algorithm>

using i32 = int;
using i64 = long long;
using i128 = __int128_t;
using u32 = unsigned int;
using u64 = unsigned long long;
using u128 = __uint128_t;

template<typename T> using vec = std::vector<T>;
using pii = std::pair<int, int>;

/**
 * @brief Internal Definition
 * 
 */
#include <queue>

/**
 * @brief Double Ended Priority Queue
 * 
 * @tparam T    elements' type
 * @tparam Comp compare for min opt
 */
template<typename T, 
    class MinComp = std::less<T>, class MaxComp = std::greater<T>>
class DEPQ
{
private:
    u32 sz;
    vec<T> a;
    vec<char> del;

    template<typename C>
    struct PosComp
    {
        const vec<T> &v;
        C comp;
        explicit PosComp(const vec<T> &u, C c): v(u), comp(c) { }
        bool operator()(u32 x, u32 y) { return comp(v[x], v[y]); }
    };

    std::priority_queue<u32, vec<u32>, PosComp<MinComp>> mnh; // min heap
    std::priority_queue<u32, vec<u32>, PosComp<MaxComp>> mxh; // max heap


    void fresh_min() { while (!mnh.empty() && del[mnh.top()]) mnh.pop(); }
    void fresh_max() { while (!mxh.empty() && del[mxh.top()]) mxh.pop(); }
    
public:
    DEPQ(const MinComp &mnc = MinComp(), const MaxComp &mxc = MaxComp())
        : sz(), a(), del(), mnh(PosComp(a, mnc)), mxh(PosComp(a, mxc)) { }
    template<typename It> DEPQ(It first, It second, 
        const MinComp &mnc = MinComp(), const MaxComp &mxc = MaxComp())
        : sz(second - first), a(first, second), del(sz), 
          mnh([this, mnc](){
            std::vector<u32> b(this->size()); 
            for (u32 i = 0; i < this->size(); i++) b[i] = i; 
            return std::priority_queue(b.begin(), b.end(), PosComp(a, mnc));
          }()), mxh([this, mxc](){
            std::vector<u32> b(this->size()); 
            for (u32 i = 0; i < this->size(); i++) b[i] = i; 
            return std::priority_queue(b.begin(), b.end(), PosComp(a, mxc));
          }()) { }


    T min() { return fresh_min(), a[mnh.top()]; }
    T max() { return fresh_max(), a[mxh.top()]; }

    constexpr u32 size() { return sz; }

    void pop_min()
    {
        fresh_min();
        del[mnh.top()] = true;
        mnh.pop();
        --sz;
    }

    void pop_max()
    {
        fresh_max();
        del[mxh.top()] = true;
        mxh.pop();
        --sz;
    }

    void push(const T &x)
    {
        a.emplace_back(x);
        del.emplace_back();
        mnh.push(a.size() - 1u);
        mxh.push(a.size() - 1u);
        ++sz;
    }
};

signed main()
{
    using namespace std;
    ios::sync_with_stdio(false); cin.tie(0), cout.tie(0);
    u32 n, q;
    cin >> n >> q;
    vec<int> s(n);
    for (int &i: s) cin >> i;
    sort(s.begin(), s.end());
    DEPQ<int> dq(s.begin(), s.end());
    u32 op, x;
    while (q--) {
        cin >> op;
        if (op == 1) {
            cout << dq.max() << '\n';
            dq.pop_max();
        }
        else if (op == 2) {
            cout << dq.min() << '\n';
            dq.pop_min();
        }
        else {
            cin >> x;
            dq.push(x);
        }
    }
}