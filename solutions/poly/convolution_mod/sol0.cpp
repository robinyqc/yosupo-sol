#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <atcoder/modint>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

using mint = atcoder::modint998244353;

#define __m (1u << n)

u32 rev(u32 x, u32 n)
{
    u32 res = 0;
    for (u32 i = 0; i < n; i++) res |= ((x >> i) & 1) << (n - i);
    return res >> 1;
}

void fft(u32 n, vec<mint> &a)
{
    for (u32 i = 0; i < __m; i++) if (i < rev(i, n)) {
        swap(a[i], a[rev(i, n)]);
    }
    for (u32 len = 1; len < __m; len *= 2) {
        mint base(mint::raw(3).pow(499122176u / len));
        for (u32 i = 0; i < __m; i += len * 2) {
            mint omega(mint::raw(1));
            for (u32 j = i; j < i + len; j++) {
                mint u(a[j]), v(a[j + len] * omega);
                a[j] = u + v;
                a[j + len] = u - v;
                omega *= base;
            }
        }
    }
}

vec<mint> convolution(vec<mint> a, vec<mint> b)
{
    u32 n = 0;
    while (__m < max(a.size(), b.size())) n++;
    n++;
    a.resize(__m), b.resize(__m);
    fft(n, a), fft(n, b);
    for (u32 i = 0; i < __m; i++) a[i] *= b[i];
    fft(n, a);
    mint inv(mint::raw(__m).inv());
    for (auto &i: a) i *= inv;
    reverse(a.begin() + 1, a.end());
    return a;
}

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, m;
    cin >> n >> m;
    vec<mint> a(n), b(m);
    u32 v;
    for (auto &i: a) {
        cin >> v;
        i = mint::raw(v);
    }
    for (auto &i: b) {
        cin >> v;
        i = mint::raw(v);
    }
    a = convolution(std::move(a), std::move(b));
    for (u32 i = 0; i < n + m - 1; i++) cout << a[i].val() << ' ';
    cout << endl;
    return 0;
}