// any modular

#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <cstdint>
#include <array>
#include <cassert>
#include <immintrin.h>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

// reference: nyaannyaan
// https://nyaannyaan.github.io/library/
namespace nyaan
{

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2")

template <typename Int, typename UInt, typename Long, typename ULong, int id>
struct ArbitraryLazyMontgomeryModIntBase {
    using mint = ArbitraryLazyMontgomeryModIntBase;

    inline static UInt mod;
    inline static UInt r;
    inline static UInt n2;
    static constexpr int bit_length = sizeof(UInt) * 8;

    static UInt get_r() {
        UInt ret = mod;
        while (mod * ret != 1) ret *= UInt(2) - mod * ret;
        return ret;
    }
    static void set_mod(UInt m) {
        assert(m < (UInt(1u) << (bit_length - 2)));
        assert((m & 1) == 1);
        mod = m, n2 = -ULong(m) % m, r = get_r();
    }
    UInt a;

    ArbitraryLazyMontgomeryModIntBase() : a(0) {}
    ArbitraryLazyMontgomeryModIntBase(const Long &b)
            : a(reduce(ULong(b % mod + mod) * n2)){};

    static UInt reduce(const ULong &b) {
        return (b + ULong(UInt(b) * UInt(-r)) * mod) >> bit_length;
    }

    mint &operator+=(const mint &b) {
        if (Int(a += b.a - 2 * mod) < 0) a += 2 * mod;
        return *this;
    }
    mint &operator-=(const mint &b) {
        if (Int(a -= b.a) < 0) a += 2 * mod;
        return *this;
    }
    mint &operator*=(const mint &b) {
        a = reduce(ULong(a) * b.a);
        return *this;
    }
    mint &operator/=(const mint &b) {
        *this *= b.inverse();
        return *this;
    }

    mint operator+(const mint &b) const { return mint(*this) += b; }
    mint operator-(const mint &b) const { return mint(*this) -= b; }
    mint operator*(const mint &b) const { return mint(*this) *= b; }
    mint operator/(const mint &b) const { return mint(*this) /= b; }

    bool operator==(const mint &b) const {
        return (a >= mod ? a - mod : a) == (b.a >= mod ? b.a - mod : b.a);
    }
    bool operator!=(const mint &b) const {
        return (a >= mod ? a - mod : a) != (b.a >= mod ? b.a - mod : b.a);
    }
    mint operator-() const { return mint(0) - mint(*this); }
    mint operator+() const { return mint(*this); }

    mint pow(ULong n) const {
        mint ret(1), mul(*this);
        while (n > 0) {
            if (n & 1) ret *= mul;
            mul *= mul, n >>= 1;
        }
        return ret;
    }

    friend ostream &operator<<(ostream &os, const mint &b) {
        return os << b.get();
    }

    friend istream &operator>>(istream &is, mint &b) {
        Long t;
        is >> t;
        b = ArbitraryLazyMontgomeryModIntBase(t);
        return (is);
    }

    mint inverse() const {
        Int x = get(), y = get_mod(), u = 1, v = 0;
        while (y > 0) {
            Int t = x / y;
            swap(x -= t * y, y);
            swap(u -= t * v, v);
        }
        return mint{u};
    }

    UInt get() const {
        UInt ret = reduce(a);
        return ret >= mod ? ret - mod : ret;
    }

    static UInt get_mod() { return mod; }
};

// id に適当な乱数を割り当てて使う
template <int id>
using ArbitraryLazyMontgomeryModInt =
        ArbitraryLazyMontgomeryModIntBase<int, unsigned int, long long,
                                          unsigned long long, id>;
template <int id>
using ArbitraryLazyMontgomeryModInt64bit =
        ArbitraryLazyMontgomeryModIntBase<
        long long, unsigned long long, __int128_t, __uint128_t, id>;

using m256 = __m256i;
struct alignas(32) mmint {
    m256 x;
    static mmint R, M0, M1, M2, N2;

    mmint() : x() {}
    inline mmint(const m256& _x) : x(_x) {}
    inline mmint(unsigned int a) : x(_mm256_set1_epi32(a)) {}
    inline mmint(unsigned int a0, unsigned int a1, unsigned int a2,
                             unsigned int a3, unsigned int a4, unsigned int a5,
                             unsigned int a6, unsigned int a7)
            : x(_mm256_set_epi32(a7, a6, a5, a4, a3, a2, a1, a0)) { }
    inline operator m256&() { return x; }
    inline operator const m256&() const { return x; }
    inline int& operator[](int i) { return *(reinterpret_cast<int*>(&x) + i); }
    inline const int& operator[](int i) const {
        return *(reinterpret_cast<const int*>(&x) + i);
    }

    // modified
    array<u32, 8> get_val() {
        unsigned r = R[0], mod = M1[0];
        auto reduce1 = [&](const uint64_t& b) {
            unsigned res = 
                (b + uint64_t(unsigned(b) * unsigned(-r)) * mod) >> 32;
            return res >= mod ? res - mod : res;
        };
        array<u32, 8> res;
        for (int i = 0; i < 8; i++) {
            res[i] = reduce1((*this)[i]);
        }
        return res;
    }
    friend ostream& operator<<(ostream& os, const mmint& m) {
        unsigned r = R[0], mod = M1[0];
        auto reduce1 = [&](const uint64_t& b) {
        unsigned res = (b + uint64_t(unsigned(b) * unsigned(-r)) * mod) >> 32;
            return res >= mod ? res - mod : res;
        };
        for (int i = 0; i < 8; i++) {
            os << reduce1(m[i]) << (i == 7 ? "" : " ");
        }
        return os;
    }

    template <typename mint>
    static void set_mod() {
        R = _mm256_set1_epi32(mint::r);
        M0 = _mm256_setzero_si256();
        M1 = _mm256_set1_epi32(mint::get_mod());
        M2 = _mm256_set1_epi32(mint::get_mod() * 2);
        N2 = _mm256_set1_epi32(mint::n2);
    }

    static inline mmint reduce(const mmint& prod02, const mmint& prod13) {
        m256 unpalo = _mm256_unpacklo_epi32(prod02, prod13);
        m256 unpahi = _mm256_unpackhi_epi32(prod02, prod13);
        m256 prodlo = _mm256_unpacklo_epi64(unpalo, unpahi);
        m256 prodhi = _mm256_unpackhi_epi64(unpalo, unpahi);
        m256 hiplm1 = _mm256_add_epi32(prodhi, M1);
        m256 prodlohi = _mm256_shuffle_epi32(prodlo, 0xF5);
        m256 lmlr02 = _mm256_mul_epu32(prodlo, R);
        m256 lmlr13 = _mm256_mul_epu32(prodlohi, R);
        m256 prod02_ = _mm256_mul_epu32(lmlr02, M1);
        m256 prod13_ = _mm256_mul_epu32(lmlr13, M1);
        m256 unpalo_ = _mm256_unpacklo_epi32(prod02_, prod13_);
        m256 unpahi_ = _mm256_unpackhi_epi32(prod02_, prod13_);
        m256 prod = _mm256_unpackhi_epi64(unpalo_, unpahi_);
        return _mm256_sub_epi32(hiplm1, prod);
    }

    static inline mmint itom(const mmint& A) { return A * N2; }

    static inline mmint mtoi(const mmint& A) {
        m256 A13 = _mm256_shuffle_epi32(A, 0xF5);
        m256 lmlr02 = _mm256_mul_epu32(A, R);
        m256 lmlr13 = _mm256_mul_epu32(A13, R);
        m256 prod02_ = _mm256_mul_epu32(lmlr02, M1);
        m256 prod13_ = _mm256_mul_epu32(lmlr13, M1);
        m256 unpalo_ = _mm256_unpacklo_epi32(prod02_, prod13_);
        m256 unpahi_ = _mm256_unpackhi_epi32(prod02_, prod13_);
        m256 prod = _mm256_unpackhi_epi64(unpalo_, unpahi_);
        m256 cmp = _mm256_cmpgt_epi32(prod, M0);
        m256 dif = _mm256_and_si256(cmp, M1);
        return _mm256_sub_epi32(dif, prod);
    }

    friend inline mmint operator+(const mmint& A, const mmint& B) {
        m256 apb = _mm256_add_epi32(A, B);
        m256 ret = _mm256_sub_epi32(apb, M2);
        m256 cmp = _mm256_cmpgt_epi32(M0, ret);
        m256 add = _mm256_and_si256(cmp, M2);
        return _mm256_add_epi32(add, ret);
    }

    friend inline mmint operator-(const mmint& A, const mmint& B) {
        m256 ret = _mm256_sub_epi32(A, B);
        m256 cmp = _mm256_cmpgt_epi32(M0, ret);
        m256 add = _mm256_and_si256(cmp, M2);
        return _mm256_add_epi32(add, ret);
    }

    friend inline mmint operator*(const mmint& A, const mmint& B) {
        m256 a13 = _mm256_shuffle_epi32(A, 0xF5);
        m256 b13 = _mm256_shuffle_epi32(B, 0xF5);
        m256 prod02 = _mm256_mul_epu32(A, B);
        m256 prod13 = _mm256_mul_epu32(a13, b13);
        return reduce(prod02, prod13);
    }

    inline mmint& operator+=(const mmint& A) { return (*this) = (*this) + A; }
    inline mmint& operator-=(const mmint& A) { return (*this) = (*this) - A; }
    inline mmint& operator*=(const mmint& A) { return (*this) = (*this) * A; }

    bool operator==(const mmint& A) {
        m256 sub = _mm256_sub_epi32(x, A.x);
        return _mm256_testz_si256(sub, sub) == 1;
    }
    bool operator!=(const mmint& A) { return !((*this) == A); }
};
__attribute__((aligned(32))) mmint mmint::R;
__attribute__((aligned(32))) mmint mmint::M0, mmint::M1, mmint::M2, mmint::N2;

}


template<typename mint>
mint factorial(u32 n)
{
    using vmint = nyaan::mmint;
    vmint::set_mod<mint>();

    vmint a(mint(1).a, mint(2).a, mint(3).a, mint(4).a, 
            mint(5).a, mint(6).a, mint(7).a, mint(8).a);
    vmint b(mint(8).a, mint(8).a, mint(8).a, mint(8).a, 
            mint(8).a, mint(8).a, mint(8).a, mint(8).a);
    vmint c(mint(1).a, mint(1).a, mint(1).a, mint(1).a, 
            mint(1).a, mint(1).a, mint(1).a, mint(1).a);

    u32 m = n / 8 * 8;
    for (u32 i = 0; i < m; i += 8) {
        c *= a;
        a += b;
    }

    auto val(c.get_val());
    mint res = 1;
    for (u32 i: val) res *= i;

    for (u32 i = m + 1; i <= n; i++) res *= i;
    return res;
}

signed main() 
{
    using mint = nyaan::ArbitraryLazyMontgomeryModInt<1>;

    mint::set_mod(998244353);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        cout << factorial<mint>(n) << endl;
    }
    return 0; 
}