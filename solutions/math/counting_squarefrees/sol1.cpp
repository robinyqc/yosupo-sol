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

// Couting square free integers.
// Init: O(sqrt(n))
// Query: O(n^{1/3}) per query.
// [WARNING] Cannot pass lib checker's tests. Memory usage is too large.
// Verification: https://www.spoj.com/problems/SQFREE/
// Submission ID: 33525399
struct SquareFrees
{
    vec<int> mer; ///< table of Mertens function
    SquareFrees(u64 maxn): mer(mobius_table(ceil(sqrtl(maxn))))
    {
        for (u32 i = 1; i < mer.size(); i++) mer[i] += mer[i - 1];
    }

    u64 count(u64 n)
    {
        i64 res = 0;
        for (u64 l = 1, r, v; l * l <= n; l = r + 1) {
            v = n / (l * l);
            r = sqrtl(n / v);
            res += i64(v) * i64(mer[r] - mer[l - 1]);
        }
        return res;
    }
};

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    SquareFrees sqc(1e14);
    u64 n;
    cin >> n;
    cout << sqc.count(n) << endl;
    return 0;
}