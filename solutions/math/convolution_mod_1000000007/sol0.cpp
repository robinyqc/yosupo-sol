// Karatsuba: convolution without transformation (so it's accurate).
// Complexity: O(n^{1.58})
// Optimized using ptr instead of vec. USING SIMD.

#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <cassert>
#include <immintrin.h>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

// From Nyaan's lib
template <u32 mod>
struct LazyMontgomeryModInt {
    using mint = LazyMontgomeryModInt;

    static constexpr u32 get_r()
    {
        u32 ret = mod;
        for (i32 i = 0; i < 4; ++i)
            ret *= 2 - mod * ret;
        return ret;
    }

    static constexpr u32 r = get_r();
    static constexpr u32 n2 = -u64(mod) % mod;
    static_assert(mod < (1 << 30), "invalid, mod >= 2 ^ 30");
    static_assert((mod & 1) == 1, "invalid, mod % 2 == 0");
    static_assert(r * mod == 1, "this code has bugs.");

    u32 a;

    constexpr LazyMontgomeryModInt()
        : a(0)
    {
    }
    constexpr LazyMontgomeryModInt(const i64& b)
        : a(reduce(u64(b % mod + mod) * n2)) {};

    static constexpr u32 reduce(const u64& b)
    {
        return (b + u64(u32(b) * u32(-r)) * mod) >> 32;
    }

    constexpr mint& operator+=(const mint& b)
    {
        if (i32(a += b.a - 2 * mod) < 0)
            a += 2 * mod;
        return *this;
    }

    constexpr mint& operator-=(const mint& b)
    {
        if (i32(a -= b.a) < 0)
            a += 2 * mod;
        return *this;
    }

    constexpr mint& operator*=(const mint& b)
    {
        a = reduce(u64(a) * b.a);
        return *this;
    }

    constexpr mint& operator/=(const mint& b)
    {
        *this *= b.inverse();
        return *this;
    }
    // modified
    friend constexpr mint operator+(mint a, mint b) { return a += b; }
    friend constexpr mint operator-(mint a, mint b) { return a -= b; }
    friend constexpr mint operator*(mint a, mint b) { return a *= b; }
    friend constexpr mint operator/(mint a, mint b) { return a /= b; }
    constexpr bool operator==(const mint& b) const
    {
        return (a >= mod ? a - mod : a) == (b.a >= mod ? b.a - mod : b.a);
    }
    constexpr bool operator!=(const mint& b) const
    {
        return (a >= mod ? a - mod : a) != (b.a >= mod ? b.a - mod : b.a);
    }
    constexpr mint operator-() const { return mint() - mint(*this); }
    constexpr mint operator+() const { return mint(*this); }

    constexpr mint pow(u64 n) const
    {
        mint ret(1), mul(*this);
        while (n > 0) {
            if (n & 1)
                ret *= mul;
            mul *= mul;
            n >>= 1;
        }
        return ret;
    }

    constexpr mint inverse() const
    {
        int x = get(), y = mod, u = 1, v = 0, t = 0, tmp = 0;
        while (y > 0) {
            t = x / y;
            x -= t * y, u -= t * v;
            tmp = x, x = y, y = tmp;
            tmp = u, u = v, v = tmp;
        }
        return mint { u };
    }

    friend ostream& operator<<(ostream& os, const mint& b)
    {
        return os << b.get();
    }

    friend istream& operator>>(istream& is, mint& b)
    {
        int64_t t;
        is >> t;
        b = LazyMontgomeryModInt<mod>(t);
        return (is);
    }

    constexpr u32 get() const
    {
        u32 ret = reduce(a);
        return ret >= mod ? ret - mod : ret;
    }

    static constexpr u32 get_mod() { return mod; }
};

__attribute__((target("avx2"))) inline __m256i my256_mullo_epu32(
    const __m256i& a, const __m256i& b)
{
    return _mm256_mullo_epi32(a, b);
}

__attribute__((target("avx2"))) inline __m256i my256_mulhi_epu32(
    const __m256i& a, const __m256i& b)
{
    __m256i a13 = _mm256_shuffle_epi32(a, 0xF5);
    __m256i b13 = _mm256_shuffle_epi32(b, 0xF5);
    __m256i prod02 = _mm256_mul_epu32(a, b);
    __m256i prod13 = _mm256_mul_epu32(a13, b13);
    __m256i prod = _mm256_unpackhi_epi64(_mm256_unpacklo_epi32(prod02, prod13),
        _mm256_unpackhi_epi32(prod02, prod13));
    return prod;
}

__attribute__((target("avx2"))) inline __m256i montgomery_mul_256(
    const __m256i& a, const __m256i& b, const __m256i& r, const __m256i& m1)
{
    return _mm256_sub_epi32(
        _mm256_add_epi32(my256_mulhi_epu32(a, b), m1),
        my256_mulhi_epu32(my256_mullo_epu32(my256_mullo_epu32(a, b), r), m1));
}

__attribute__((target("avx2"))) inline __m256i montgomery_add_256(
    const __m256i& a, const __m256i& b, const __m256i& m2, const __m256i& m0)
{
    __m256i ret = _mm256_sub_epi32(_mm256_add_epi32(a, b), m2);
    return _mm256_add_epi32(_mm256_and_si256(_mm256_cmpgt_epi32(m0, ret), m2),
        ret);
}

template<typename Mint>
__attribute__((target("avx2"))) 
void vectorized_convolution(Mint* a, Mint* b, Mint* c)
{
    const __m256i m0 = _mm256_set1_epi32(0);
    const __m256i m1 = _mm256_set1_epi32(Mint::get_mod());
    const __m256i m2 = _mm256_set1_epi32(Mint::get_mod() * 2u);
    const __m256i r = _mm256_set1_epi32(Mint::r);

    __m256i va, vb, res, dst;

    for (u32 i = 0; i < 32; i++) {
        va = _mm256_set1_epi32(a[i].a);
        for (u32 j = 0; j < 32; j += 8) {
            vb = _mm256_loadu_si256((__m256i_u*)&b[j].a);
            res = montgomery_mul_256(va, vb, r, m1);
            dst = _mm256_loadu_si256((__m256i_u*)&c[i + j].a);
            _mm256_storeu_si256((__m256i_u*)&c[i + j].a, 
                montgomery_add_256(dst, res, m2, m0));
        }
    }
}

template<typename T>
void convolution_karatsuba_mont_avx_rec(u32 n, T* a, T* b, T* dst)
{
    if (n <= 32) {
        if (n <= 16) {
            for (u32 i = 0; i < n; i++) {
                for (u32 j = 0; j < n; j++) {
                    dst[i + j] += a[i] * b[j];
                }
            }
            return ;
        }
        vectorized_convolution(a, b, dst);
        return ;
    }

    u32 m = n / 2;

    T* fab = new T[m];
    memcpy(fab, a, m * sizeof(T));
    for (u32 i = 0; i < m; i++) fab[i] += a[i + m];

    #define fac dst
    T* fcd = new T[m];
    memcpy(fcd, b, m * sizeof(T));
    for (u32 i = 0; i < m; i++) fcd[i] += b[i + m];

    T* fbd = dst + 2 * m;
    convolution_karatsuba_mont_avx_rec<T>(m, a, b, fac);
    convolution_karatsuba_mont_avx_rec<T>(m, a + m, b + m, fbd);

    T* fabcd = new T[2 * m - 1]();
    convolution_karatsuba_mont_avx_rec<T>(m, fab, fcd, fabcd);
    delete []fab;
    delete []fcd;

    for (u32 i = 0; i + 1 < 2 * m; i++) fabcd[i] -= fac[i];
    for (u32 i = 0; i + 1 < 2 * m; i++) fabcd[i] -= fbd[i];

    for (u32 i = 0; i + 1 < 2 * m; i++) fac[i + m] += fabcd[i];
    delete []fabcd;
    #undef fac
}

#define MONT_T(x) LazyMontgomeryModInt<x>
template<u32 mod>
vec<MONT_T(mod)> convolution_karatsuba_mont_avx
    (vec<MONT_T(mod)> a, vec<MONT_T(mod)> b)
{
    u32 n = a.size(), m = b.size();
    if (n < m) swap(n, m), a.resize(n), m += n;
    else b.resize(n), m += n;
    u32 q = 1;
    while (q < n) q += q;
    a.resize(q), b.resize(q);
    MONT_T(mod) *res = new MONT_T(mod)[2 * q - 1]();
    convolution_karatsuba_mont_avx_rec(q, a.data(), b.data(), res);
    vec<MONT_T(mod)> c(res, res + m - 1);
    delete []res;
    return c;
}

// functional format input
vec<int> solve(vec<int> a, vec<int> b)
{
    using mint = LazyMontgomeryModInt<1000000007>;
    auto t(convolution_karatsuba_mont_avx<1000000007>
        ({a.begin(), a.end()}, {b.begin(), b.end()}));
    a.resize(t.size());
    for (u32 i = 0, n = t.size(); i < n; i++) a[i] = t[i].get();
    return a;
}

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, m;
    cin >> n >> m;
    
    vec<int> a(n), b(m);
    for (auto &i: a) cin >> i;
    for (auto &i: b) cin >> i;

    a = solve(std::move(a), std::move(b));
    for (auto i: a) cout << i << ' ';
    cout << endl;
    return 0;
}