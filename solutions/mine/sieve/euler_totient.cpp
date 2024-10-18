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

vec<u32> euler_totient_table(u32 n)
{
    vec<u32> prime, phi(n + 1);
    vec<bool> vis(n + 1);

    phi[1] = 1;

    for (u32 i = 2; i <= n; i++) {
        if (!vis[i]) {
            phi[i] = i - 1;
            prime.emplace_back(i);
        }
        for (u32 j: prime) {
            if (i * j > n) break;
            vis[i * j] = true;
            if (i % j == 0) {
                phi[i * j] = phi[i] * j;
                break;
            }
            phi[i * j] = phi[i] * phi[j];
        }
    }

    return phi;
}

signed main() 
{
    vec<u32> t = euler_totient_table(20);
    for (u32 i = 1; i <= 20; i++) cout << t[i] << ' ';
    cout << endl;
    return 0;
}

/*
Expected:
OEIS A000010
1, 1, 2, 2, 4, 2, 6, 4, 6, 4, 10, 4, 12, 6, 8, 8, 16, 6, 18, 8
*/