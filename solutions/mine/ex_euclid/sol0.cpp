#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <numeric>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

// ax + by = gcd(a, b)
u32 ex_euclid(u32 a, u32 b, i32 &x, i32 &y)
{
    x = 1, y = 0;
    i32 n(0), m(1), t, q;
    while (b) {
        q = a / b;
        t = n, n = x - q * n, x = t;
        t = m, m = y - q * m, y = t;
        t = b, b = a - q * b, a = t;
    }
    return a;
}
u64 ex_euclid(u64 a, u64 b, i64 &x, i64 &y)
{
    x = 1, y = 0;
    i64 n(0), m(1), t, q;
    while (b) {
        q = a / b;
        t = n, n = x - q * n, x = t;
        t = m, m = y - q * m, y = t;
        t = b, b = a - q * b, a = t;
    }
    return a;
}

signed main() 
{
    i64 a = 95545765675398584LL, b = 612062871820861916LL, x, y;
    cout << ex_euclid(a, b, x, y) << endl; // 4099732
    cout << x << ' ' << y << endl; // -44592092119 6961026033
    cout << x * a + y * b << endl;
    return 0;
}