#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <cmath>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

vec<int> mobius_table(u32 n)
{
    vec<u32> prime;
    vec<int> mu(n + 1);
    vec<bool> vis(n + 1);
    prime.reserve(n / 15);
    mu[1] = 1;
    for (u32 i = 2; i <= n; i++) {
        if (!vis[i]) {
            mu[i] = -1;
            prime.emplace_back(i);
        }
        for (u32 j: prime) {
            if (i * j > n) break;
            vis[i * j] = true;
            if (i % j == 0) {
                mu[i * j] = 0;
                break;
            }
            mu[i * j] = -mu[i];
        }
    }
    return mu;
}

u64 floor_sqrt(u64 x)
{
    static u64 y;
    y = sqrt(x);
    while (y * y < x) ++y;
    while (y * y > x) --y;
    return y;
}

// time complexity: O(n^{2/5})
// space complexity: O(n^{2/5})
// reference: https://smsxgz.github.io/post/pe/counting_square_free_numbers/
//            https://arxiv.org/pdf/1107.4890
u64 count_square_free_integers(u64 n)
{
    const u32 I = ceil(powl(n, 0.2)), D = floor_sqrt(n / I);
    auto mu(mobius_table(D));

    // Calculate s1.
    i64 s1 = 0;
    for (u64 i = 1; i <= D; i++) {
        s1 += mu[i] * i64(n / (i * i));
    }

    // Calculate the table of Mertens function from 1 to D and store it in mu.
    for (u32 i = 1; i <= D; i++) mu[i] += mu[i - 1];

    // Calculate s2.
    i64 s2 = 0;
    vec<i64> mxi(I);
    for (u32 i = I - 1; i > 0; i--) {
        u32 x = floor_sqrt(n / i);
        i64 val = 1;
        
        for (u32 l = 2, r, v; l <= x; l = r + 1) {
            v = x / l;
            r = x / v;
            if (v <= D) val -= i64(r - l + 1) * mu[v];
            else {
                // val -= (r - l + 1) * mxi[l * l * i]; // Actually r == l.
                val -= mxi[l * l * i];
            }
        }
        
        s2 += val;
        mxi[i] = val;
    }

    s2 -= i64(I - 1) * mu[D];

    return s1 + s2;
}

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u64 n;
    cin >> n;
    cout << count_square_free_integers(n) << endl;
    return 0;
}