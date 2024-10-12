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

template<typename T, u32 B>
struct SqrtRAArray
{
    vec<T> bl, t;
    SqrtRAArray(): bl(), t() { }
    SqrtRAArray(u32 n): bl(n / B + 1), t(n + 1) { }

    void add(u32 r, T d)
    {
        u32 x = 0;
        for (; (x + 1) * B <= r; x++) bl[x] += d;
        for (x *= B; x < r; x++) t[x] += d;
    }
    
    void add(u32 l, u32 r, T d) { add(l, -d), add(r, d); }

    T get(u32 x) { return bl[x / B] + t[x]; }
};

signed main() 
{
    int n, q, op, x, y, z;
    cin >> n >> q;
    SqrtRAArray<i64, 333> t(n);
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