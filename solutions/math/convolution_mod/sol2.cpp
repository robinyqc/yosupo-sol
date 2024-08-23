#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <bit>
#include <atcoder/modint>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

namespace poly
{

namespace details
{

vec<u32> r[30];
const vec<u32>& calc(const u32 n)
{
    if (r[n].size()) return r[n];
    u32 q = 1 << n, p = q / 2;
    r[n].resize(q);
    for (u32 i = 1; i < q; i++) {
        r[n][i] = r[n][i / 2] / 2;
        if (i % 2) r[n][i] += p;
    }
    return r[n];
}

}

template<typename T> void reverse_transform(const u32 n, vec<T> &a)
{
    const auto &r = details::calc(n);
    u32 q = 1 << n;
    for (u32 i = 0; i < q; i++) {
        if (i < r[i]) swap(a[i], a[r[i]]);
    }
}

template<bool d, class B, class I, typename T>
void fourier_transform(const u32 n, vec<T> &a, B base, I inv, T e)
{
    reverse_transform(n, a);
    u32 q = 1 << n;
    for (u32 h = 2; h <= q; h *= 2) {
        T b = base(h);
        for (u32 j = 0; j < q; j += h) {
            T w(e);
            for (u32 k = j, l = h / 2; k < j + l; k++) {
                T u = a[k], v = w * a[k + l];
                a[k] = u + v;
                a[k + l] = u - v;
                w = w * b;
            }
        }
    }
    if (d) {
        for (u32 i = 0; i < q; i++) inv(a[i], q);
    }
}

}

signed main() 
{
    ios::sync_with_stdio(false); cin.tie(0), cout.tie(0);
    using mint = atcoder::modint998244353;
    auto read_int = [x = int()]() mutable { cin >> x; return x; };
    u32 n = read_int(), m = read_int();
    u32 p = bit_width(max(bit_ceil(n), bit_ceil(m))), q = 1 << p;
    vec<mint> a(q), b(q);
    for (u32 i = 0; i < n; i++) a[i] = read_int();
    for (u32 i = 0; i < m; i++) b[i] = read_int();
    auto base = [](u32 h) { return mint::raw(3).pow(998244352 / h); };
    auto inv = [iv2 = mint(q).inv()](mint &x,[[maybe_unused]] u32 y){x *= iv2;};
    poly::fourier_transform<0>(p, a, base, inv, mint::raw(1));
    poly::fourier_transform<0>(p, b, base, inv, mint::raw(1));
    for (u32 i = 0; i < q; i++) a[i] *= b[i];
    poly::fourier_transform<1>(p, a, base, inv, mint::raw(1));
    reverse(a.begin() + 1, a.end());
    for (u32 i = 0; i < n + m - 1; i++) cout << a[i].val() << ' ';
    cout << '\n';
    return 0;
}