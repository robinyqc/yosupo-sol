#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <optional>
#include <random>
#include <atcoder/convolution>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

template<typename T, int id = -1> struct ExField
{
    static T i2; // i * i
    static void set_i2(T _i2) { i2 = _i2; }
    T real, imag;
    ExField(T _real = 0, T _imag = 0): real(_real), imag(_imag) { }
    ExField operator+(const ExField &t) const 
        { return {real + t.real, imag + t.imag}; }
    ExField operator*(const ExField &t) const 
    { 
        return {real * t.real + imag * t.imag * i2, 
                real * t.imag + imag * t.real}; 
    }
};
template<typename T, int id> T ExField<T, id>::i2 = 0;

template<typename T> T qpow(T x, u64 y, T un = static_cast<T>(1))
{
    for (; y; y >>= 1) {
        if (y & 1) un = un * x;
        x = x * x;
    }
    return un;
}

// legendre(x, Mint::mod())
template<typename Mint> bool is_quadratic_residue(Mint x)
{
    return qpow(x, (Mint::mod() - 1) >> 1).val() == 1;
}

// solve the equation x * x % Mint::mod() == a
template<typename Mint>
optional<Mint> sqrt_mod(Mint a)
{
    using F = ExField<Mint, 253836377>;
    static mt19937 gen;

    if (a.val() == 0) return Mint::raw(0);
    if (Mint::mod() == 2) return Mint::raw(1);

    if (!is_quadratic_residue(a)) return nullopt;

    uniform_int_distribution<> dist(0, Mint::mod() - 1);
    Mint b;
    do {
        b = Mint::raw(dist(gen));
    } while (is_quadratic_residue(b * b - a));

    F::set_i2(b * b - a);

    return qpow(F(0, 1) + F(b), (Mint::mod() + 1) >> 1).real;
}

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

vec<mint> sqrt_poly(const vec<mint> &a, u32 tlen)
{
    static const mint inv2 = mint::raw(2).inv();
    vec<mint> res{a[0]};
    while (res.size() < tlen) {
        size_t nlen = res.size() * 2;
        vec<mint> rhs(atcoder::convolution(
            {a.begin(), a.begin() + min(nlen, a.size())}, 
            inv_poly(res, nlen)));
        res.resize(nlen);
        nlen = min(nlen, rhs.size());
        for (u32 i = 0; i < nlen; i++) {
            res[i] = inv2 * (res[i] + rhs[i]);
        }
    }
    return vec<mint>(res.begin(), res.begin() + tlen);
}

optional<vec<mint>> sqrt_poly_any(const vec<mint> &a, u32 tlen)
{
    static const mint inv2 = mint::raw(2).inv();
    u32 idx = 0, n = a.size();
    while (idx < n && a[idx].val() == 0) idx++;
    if (idx == n) return vec<mint>(tlen, mint::raw(0));
    if (idx & 1) return nullopt;
    if (auto sqidx = sqrt_mod(a[idx])) {
        vec<mint> res{sqidx.value()};
        while (res.size() + (idx >> 1) < tlen) {
            u32 nlen = res.size() * 2;
            vec<mint> rhs(atcoder::convolution(
                {a.begin() + idx, a.begin() + min(n, idx + nlen)}, 
                inv_poly(res, nlen)));
            res.resize(nlen);
            nlen = min(nlen, u32(rhs.size()));
            for (u32 i = 0; i < nlen; i++) {
                res[i] = inv2 * (res[i] + rhs[i]);
            }
        }
        res.insert(res.begin(), idx >> 1, mint::raw(0));
        return vec<mint>(res.begin(), res.begin() + tlen);    
    }
    return nullopt;
}

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n;
    cin >> n;
    vec<mint> a(n);
    for (auto &i: a) {
        static u32 x;
        cin >> x;
        i = mint::raw(x);
    }
    if (auto b = sqrt_poly_any(a, n)) {
        for (auto &i: b.value()) cout << i.val() << ' ';
        cout << endl;
    }
    else cout << "-1" << endl;
    return 0;
}