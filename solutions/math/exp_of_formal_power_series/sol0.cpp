#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <atcoder/convolution>
#include <atcoder/modint>
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

vec<mint> int_poly(vec<mint> a)
{
    static vec<mint> inv(2, mint::raw(1));
    static constexpr u64 P = mint::mod();
    u32 n = a.size();
    if (n == 0) return vec<mint>(1u, mint::raw(0));
    while (inv.size() <= n) {
        inv.emplace_back(mint::raw(P - P / inv.size()) * inv[P % inv.size()]);
    }
    a.emplace_back(a.back() * inv[n]);
    for (u32 i = n - 1; i; --i) {
        a[i] = a[i - 1] * inv[i];
    }
    a[0] = mint::raw(0);
    return a;
}

vec<mint> der_poly(vec<mint> a)
{
    u32 n = a.size();
    if (n == 0) return {};
    for (u32 i = 0; i < n - 1; i++) {
        a[i] = a[i + 1] * mint::raw(i + 1);
    }
    a.pop_back();
    return a;
}

vec<mint> log_poly(vec<mint> a, u32 tlen)
{
    auto b(der_poly(a));
    a = inv_poly(a, tlen);
    a = atcoder::convolution(b, a);
    a.resize(tlen - 1);
    return int_poly(std::move(a));
}

vec<mint> exp_poly(const vec<mint> &a, u32 tlen)
{
    vec<mint> res(1, mint::raw(1));
    while (res.size() < tlen) {
        size_t new_len = res.size() * 2;
        auto rhs(log_poly(res, new_len));
        for (u32 i = 0; i < new_len; i++) {
            rhs[i] = a[i] - rhs[i];
        }
        ++rhs[0];
        res = atcoder::convolution(res, rhs);
        res.resize(new_len);
    }
    res.resize(tlen);
    return res;
}

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, x;
    cin >> n;
    vec<mint> a(n);
    for (auto &i: a) cin >> x, i = x;
    a = exp_poly(std::move(a), n);
    for (u32 i = 0; i < n; i++) cout << a[i].val() << ' ';
    cout << endl;
    return 0;
}