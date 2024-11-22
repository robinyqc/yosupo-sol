// Cipolla
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <random>
#include <optional>
#include <atcoder/modint>
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

signed main() 
{
    using mint = atcoder::dynamic_modint<0>;
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    int t;
    cin >> t;
    u32 y, p;
    while (t--) {
        cin >> y >> p;
        mint::set_mod(p);
        if (auto x = sqrt_mod<mint>(y)) {
            cout << x.value().val() << '\n';
        }
        else cout << "-1\n";
    }
    return 0;
}