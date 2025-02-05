#include <random>
#include <ctime>
#include <utility>

struct ModInt61
{
    using u32 = unsigned int;
    using u64 = unsigned long long;
    using u128 = __uint128_t;
    static constexpr u64 P = (1ull << 61) - 1;
    using mint = ModInt61;
    u64 val;
    constexpr ModInt61(): val() { }
    constexpr ModInt61(u64 x): val(x) { }
    constexpr mint &operator+=(mint t)
    {
        val += t.val;
        if (val >= P) val -= P;
        return (*this);
    }
    constexpr mint &operator-=(mint t)
    {
        if (val < t.val) val += P;
        val -= t.val;
        return (*this);
    }
    constexpr mint &operator*=(mint t)
    {
        u128 res = static_cast<u128>(val) * static_cast<u128>(t.val);
        val = res >> 61;
        return (*this) += res & P;
    }

    constexpr mint pow(u64 n) const
    {
        mint res(1), cl(*this);
        for (; n; n >>= 1) {
            if (n & 1) res *= cl;
            cl *= cl;
        }
        return res;
    }

    constexpr mint inv() const { return pow(P - 2); }

    constexpr friend mint operator+(mint a, mint b) { return a += b; }
    constexpr friend mint operator-(mint a, mint b) { return a -= b; }
    constexpr friend mint operator*(mint a, mint b) { return a *= b; }
    constexpr friend bool operator==(mint a, mint b) { return a.val == b.val; }
    constexpr friend bool operator!=(mint a, mint b) { return a.val != b.val; }
};

unsigned long long hash_default_seed()
{
    static std::mt19937_64 gen(time(0));
    static std::uniform_int_distribution
        <unsigned long long> dist(1, (1ull << 61) - 2);
    return dist(gen);
}

struct MonoidHash
{
    using u64 = unsigned long long;
    using S = std::pair<ModInt61, ModInt61>;
    static u64 &get_base()
    {
        static u64 base = hash_default_seed();
        return base;
    }
    static void set_base(u64 base) { get_base() = base; }
    static S from_char(u64 x)
    {
        return {get_base(), x};
    }
    static constexpr S op(const S &x, const S &y)
    {
        return {x.first * y.first, x.second * y.first + y.second};
    }
    static constexpr S un() { return {1, 0}; }
    static constexpr bool cm = false;
};