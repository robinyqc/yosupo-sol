// Unverified.
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

/// @brief Calculate mobius function (mu) from 1 to n
/// @return mu[1..n] (mu[0] is undefined, but is equal to 0).
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

signed main() 
{
    vec<int> mu(mobius_table(20));
    for (int i: mu) cout << i << ' ';
    cout << endl;
    return 0;
}

// https://oeis.org/A008683