#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <atcoder/convolution>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;
using mint = atcoder::modint998244353;

// newton's method
vec<mint> inv_poly(const vec<mint> &a, u32 tlen)
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
    b = inv_poly(b, n - m + 1);
    b.resize(n - m + 1);
    a.resize(n - m + 1);
    a = atcoder::convolution(a, b);
    return {a.rend() - n + m - 1, a.rend()};
}

// a - b * q
vec<mint> poly_div_remainder(vec<mint> a, const vec<mint> &b, vec<mint> q)
{
    q = atcoder::convolution(b, q);
    for (u32 i = 0; i < q.size(); i++) a[i] -= q[i];
    while (a.size() && a.back().val() == 0) a.pop_back();
    return a;
}

vec<mint> poly_mod(const vec<mint> &a, const vec<mint> &b)
{
    return poly_div_remainder(a, b, poly_div(a, b));
}

vec<mint> poly_multipoint_eval(const vec<mint> &f, const vec<mint> &x)
{
    u32 n = x.size(), m = 1;
    while (m < n) m += m;
    vec<vec<mint>> prod(m * 2);

    auto build = [&](auto &&self, u32 l, u32 r, u32 p) -> void
    {
        if (l + 1 == r) {
            prod[p] = vec<mint>{-x[l], 1};
            return ;
        }
        u32 mid = (l + r) >> 1;
        self(self, l, mid, p * 2);
        self(self, mid, r, p * 2 + 1);
        prod[p] = atcoder::convolution(prod[p * 2], prod[p * 2 + 1]);
    };
    build(build, 0, n, 1);

    vec<mint> res(n);

    auto solve = [&](auto &&self, const vec<mint> &fn, 
        u32 l, u32 r, u32 p) -> void
    {
        if (l + 1 == r) {
            res[l] = fn.size() ? fn[0] : 0;
            return ;
        }
        u32 mid = (l + r) >> 1;
        self(self, poly_mod(fn, prod[p * 2]), l, mid, p * 2);
        self(self, poly_mod(fn, prod[p * 2 + 1]), mid, r, p * 2 + 1);
    };
    
    solve(solve, poly_mod(f, prod[1]), 0, n, 1);

    return res;
}

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, m, t;
    cin >> n >> m;
    vec<mint> f(n), x(m);
    for (auto &i: f) {
        cin >> t;
        i = mint::raw(t);
    }
    for (auto &i: x) {
        cin >> t;
        i = mint::raw(t);
    }
    auto y(poly_multipoint_eval(f, x));
    for (auto i: y) cout << i.val() << ' ';
    cout << endl;
    return 0;
}