#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

template<u32 mod, u32 bit_width = 16, u32 mod_exp = -1u>
class FastPow
{
private:
    static constexpr u32 len = 1ull << bit_width;
    static constexpr u64 mod64 = static_cast<u64>(mod);
    static constexpr u64 mod_exp64 = static_cast<u64>(mod_exp);
    u64 base;
    u32 bpow[len], spow[len];
public:

    FastPow(u64 _base = 2ull): base(_base)
    {
        bpow[0] = 1ull;
        for (u32 i = 1; i < len; i++) bpow[i] = bpow[i - 1] * base % mod64;
        spow[0] = 1ull;
        u64 sbase = bpow[len - 1] * base % mod64;
        for (u32 i = 1; i < len; i++) spow[i] = spow[i - 1] * sbase % mod64;
    }

    u64 pow(u64 exp)
    {
        if constexpr (mod_exp != -1u) exp %= mod_exp64;
        return static_cast<u64>(bpow[exp & ((1u << bit_width) - 1u)]) 
            * static_cast<u64>(spow[exp >> bit_width]) % mod64;
    }
};

signed main() 
{
    static FastPow<998244353> pw;
    cout << pw.pow(11254541U) << endl; // 2300512
    return 0;
}