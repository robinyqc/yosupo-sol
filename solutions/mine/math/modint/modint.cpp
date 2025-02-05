template<unsigned P> struct ModInt
{
    using u32 = unsigned int;
    using u64 = unsigned long long;
    using mint = ModInt;
    u32 val;
    constexpr ModInt(): val() { }
    constexpr ModInt(u32 x): val(x) { }
    constexpr mint &operator+=(mint t) 
    {
        val += t.val;
        if (val >= P) val -= P;
        return *this;
    }
    constexpr mint &operator-=(mint t)
    {
        if (val < t.val) val += P;
        val -= t.val;
        return *this;
    }
    constexpr mint &operator*=(mint t)
    {
        val = (u64)val * t.val % P;
        return *this;
    }
    constexpr mint &operator/=(mint t) { return (*this) *= t.inv(); }
    constexpr mint operator+() const { return (*this); }
    constexpr mint operator-() const { return P - val; }
    friend constexpr mint operator+(mint a, mint b) { return a += b; }
    friend constexpr mint operator-(mint a, mint b) { return a -= b; }
    friend constexpr mint operator*(mint a, mint b) { return a *= b; }
    friend constexpr mint operator/(mint a, mint b) { return a /= b; }

    constexpr mint pow(u32 y) const
    {
        u64 res = 1, x = val;
        for (; y; y >>= 1) {
            if (y & 1) res = res * x % P;
            x = x * x % P;
        }
        return mint(res);
    }

    constexpr mint inv() const { return pow(P - 2); }

    friend constexpr bool operator==(mint a, mint b) { return a.val == b.val; }
    friend constexpr bool operator!=(mint a, mint b) { return a.val != b.val; }
};