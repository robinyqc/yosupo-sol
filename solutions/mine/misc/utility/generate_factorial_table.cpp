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

struct Barrett 
{
    u64 d;
    __uint128_t m;
    Barrett(u64 _d): d(_d), m(((__uint128_t)(1) << 64) / _d) { }
    u64 reduce(u64 x)
    {
        u64 w = (m * x) >> 64;
        w = x - w * d;
        if (w >= d) return w - d;
        return w;
    }
} ;

signed main(int argc, char **argv) 
{
    if (argc < 4) {
        cerr << "Usage: generate_factorial_table mod upper_bound dist [new_line]" << endl;
        return 1;
    }
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 mod = stod(argv[1]);
    u32 upb = stod(argv[2]);
    u32 dis = stod(argv[3]);
    u32 nwl = 1e9;
    if (argc > 4) nwl = stod(argv[4]);

    Barrett bmod(mod);

    for (u64 i = 0, j = 0, v = 1, c = -1; i <= upb; i++, j++) {
        if (j == dis) j = 0;
        if (j == 0) {
            if (i != 0) cout << ", ";
            if ((++c) == nwl) c = 0, cout << '\n';
            cout << v;
        }
        v = bmod.reduce(v * (i + 1));
    }
    cout << endl;
    return 0;
}