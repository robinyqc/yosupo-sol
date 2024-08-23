#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <atcoder/modint>
#include <atcoder/convolution>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;
using mint = atcoder::modint998244353;

// newton's method
vec<mint> inv_poly(vec<mint> a, u32 tlen)
{
    vec<mint> res(1, a[0].inv());
    while (res.size() < tlen) {
        size_t new_len = res.size() * 2;
        if (new_len >= 32) {
            res.resize(new_len * 2);
            atcoder::internal::butterfly(res);
            vec<mint> tmp(a.begin(), a.begin() + min(a.size(), new_len));
            tmp.resize(new_len * 2);
            atcoder::internal::butterfly(tmp);
            for (u32 i = 0; i < new_len * 2; i++) {
                res[i] *= mint::raw(2) - tmp[i] * res[i];
            }
            atcoder::internal::butterfly_inv(res);
            res.resize(new_len);
            mint ilen = mint(new_len * 2).inv();
            for (auto &i: res) i *= ilen;
        }
        else {
            auto rhs(atcoder::convolution(
                {a.begin(), a.begin() + min(new_len, a.size())}, res));
            for (auto &i: rhs) i = -i;
            rhs[0] += mint::raw(2);
            res = atcoder::convolution(res, rhs);
            res.resize(new_len);
        }
    }
    return {res.begin(), res.begin() + tlen};
}

vec<mint> poly_div(vec<mint> a, vec<mint> b)
{
    u32 n = a.size(), m = b.size();
    if (n < m) return {};
    reverse(a.begin(), a.end());
    reverse(b.begin(), b.end());
    b = inv_poly(std::move(b), n - m + 1);
    b.resize(n - m + 1);
    a.resize(n - m + 1);
    a = atcoder::convolution(a, b);
    return {a.rend() - n + m - 1, a.rend()};
}

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, m, x;
    cin >> n >> m;
    vec<mint> a(n), b(m);
    for (auto &i: a) cin >> x, i = mint::raw(x);
    for (auto &i: b) cin >> x, i = mint::raw(x);
    auto q(poly_div(a, b));
    b = atcoder::convolution(q, b);
    for (u32 i = 0; i < b.size(); i++) a[i] -= b[i];
    while (a.size() && a.back() == 0) a.pop_back();
    cout << q.size() << ' ' << a.size() << endl;
    for (auto i: q) cout << i.val() << ' ';
    cout << endl;
    for (auto i: a) cout << i.val() << ' ';
    cout << endl;
    return 0;
}