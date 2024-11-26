#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <cassert>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

template<typename T, typename F>
vec<T> multiplicative_function_table(u32 n, F &&g)
{
    vec<u32> prime;
    vec<bool> vis(n + 1);
    prime.reserve(n / 15);
    vec<T> res(n + 1);
    res[1] = T(1);
    for (u32 i = 2, t, k, ni; i <= n; i++) {
        if (!vis[i]) {
            prime.emplace_back(i);
            res[i] = g(i, 1);
        }
        for (u32 j: prime) {
            if (i * j > n) break;
            vis[i * j] = true;
            t = i / j;
            if (i == t * j) {
                k = 1;
                do ni = t, t /= j, k++;
                while (ni == t * j);
                res[i * j] = res[ni] * g(j, k);
                break;
            }
            res[i * j] = res[i] * g(j, 1);
        }
    }
    return res;
}

template<typename T, typename F>
vec<T> completely_multiplicative_function_table(u32 n, F &&g)
{
    vec<u32> prime;
    vec<bool> vis(n + 1);
    prime.reserve(n / 15);
    vec<T> res(n + 1);
    res[1] = T(1);
    for (u32 i = 2; i <= n; i++) {
        if (!vis[i]) {
            prime.emplace_back(i);
            res[i] = g(i);
        }
        for (u32 j: prime) {
            if (i * j > n) break;
            vis[i * j] = true;
            res[i * j] = res[i] * res[j];
            if (i % j == 0) break;
        }
    }
    return res;
}

signed main() 
{

    for (auto i: multiplicative_function_table<i64>(20, [](i64 a, i64 k) 
    { 
        int b = 1;
        for (int x = 0; x < k; x++) b *= a;
        return b * b * b;
    })) cout << i << ' ';
    cout << endl;

    for (auto i: completely_multiplicative_function_table<i64>(20, [](i64 a) 
    { 
        return a * a * a;
    })) cout << i << ' ';
    cout << endl;
    return 0;
}