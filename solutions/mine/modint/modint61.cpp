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