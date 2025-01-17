// Sample Problem: https://www.luogu.com.cn/problem/P4694

// #include <iostream>
// #include "wqs.hpp"
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <queue>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

template<typename D, typename R = D, typename F, typename C = std::less<R>>
R wqs_invoke(F &&g, D a, D lb, D ub, D eps = 0, const C &comp = C{})
{
    if (eps == D(0)) eps = 1;
    else eps /= ub - lb;
    while (lb + eps <= ub) {
        D ml = lb + (ub - lb) / 2;
        D mr = ml + eps;
        R gl = g(R(ml)) + R(ml) * R(a);
        R gr = g(R(mr)) + R(mr) * R(a);
        if (comp(gl, gr)) lb = mr;
        else ub = ml;
    }
    return g(lb) + R(lb) * R(a);
}

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    int n, k;
    cin >> n >> k;
    vec<int> a(n), b(n);
    for (auto &i: a) cin >> i;
    for (auto &i: b) cin >> i;

    auto calc = [&](int y) 
    {
        priority_queue<int, vec<int>, greater<>> pq;
        i64 res = 0;
        for (int i = 0; i < n; i++) {
            pq.emplace(a[i]);
            int x = pq.top();
            if (x + b[i] - y < 0) {
                res += x + b[i] - y;
                pq.pop();
                pq.emplace(-b[i] + y);
            }
        }
        return res;
    };

    cout << wqs_invoke<int, i64>(calc, k, 1, 2e9) << endl;
    return 0;
}