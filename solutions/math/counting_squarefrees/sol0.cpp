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

// time complexity: O(sqrt(n))
// space complexity: O(sqrt(n))
// [WARNING] Cannot pass lib checker's tests. Memory usage is too large.
// Verification: https://www.spoj.com/problems/SQFREE/
// Submission ID: 33525290
u64 count_square_free_integers(u64 n)
{
    const u64 D = ceil(sqrtl(n));
    auto mu(mobius_table(D));
    i64 res = 0;
    for (u64 i = 1; i <= D; i++) {
        res += mu[i] * (n / (i * i));
    }
    return res;
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