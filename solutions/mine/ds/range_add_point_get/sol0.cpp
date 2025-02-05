#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <atcoder/fenwicktree>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

template<typename T>
struct RAArray
{
    atcoder::fenwick_tree<T> t;

    RAArray(): t() { }
    RAArray(int n): t(n + 1) { }

    void add(int l, int r, T d)
    {
        t.add(l, d);
        t.add(r, -d);
    }

    T get(int x) { return t.sum(0, x + 1); }
};

signed main() 
{
    int n, q, op, x, y, z;
    cin >> n >> q;
    RAArray<i64> t(n);
    while (q--) {
        cin >> op;
        if (op == 0) {
            cin >> x >> y >> z;
            t.add(x, y, z);
        }
        else {
            cin >> x;
            cout << t.get(x) << '\n';
        }
    }
    return 0;
}