// Karatsuba: convolution without transformation (so it's accurate).
// Complexity: O(n^{1.58})

#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <atcoder/modint>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

template<typename T>
void convolution_karatsuba_rec(u32 n, T* a, T* b, T* dst)
{
    if (n <= 16) {
        u32 m = n / 4 * 4;
        for (u32 i = 0; i < n; i++) {
            for (u32 j = 0; j < m; j += 4) {
                dst[i + j] += a[i] * b[j];
                dst[i + j + 1] += a[i] * b[j + 1];
                dst[i + j + 2] += a[i] * b[j + 2];
                dst[i + j + 3] += a[i] * b[j + 3];
            }
            for (u32 j = m; j < n; j++) {
                dst[i + j] += a[i] * b[j];
            }
        }

        return ;
    }

    u32 m = n / 2, q = n - m;

    T* fab = new T[q];
    memcpy(fab, a, m * sizeof(T));
    for (u32 i = 0; i < m; i++) fab[i] += a[i + m];
    if (m != q) fab[m] = a[n - 1];

    #define fac dst
    T* fcd = new T[q];
    memcpy(fcd, b, m * sizeof(T));
    for (u32 i = 0; i < m; i++) fcd[i] += b[i + m];
    if (m != q) fcd[m] = b[n - 1];

    T* fbd = dst + 2 * m;
    convolution_karatsuba_rec<T>(m, a, b, fac);
    convolution_karatsuba_rec<T>(q, a + m, b + m, fbd);

    T* fabcd = new T[2 * q - 1]();
    convolution_karatsuba_rec<T>(q, fab, fcd, fabcd);
    delete []fab;
    delete []fcd;

    for (u32 i = 0; i + 1 < 2 * m; i++) fabcd[i] -= fac[i];
    for (u32 i = 0; i + 1 < 2 * q; i++) fabcd[i] -= fbd[i];

    for (u32 i = 0; i + 1 < 2 * q; i++) fac[i + m] += fabcd[i];
    delete []fabcd;
    #undef fac
}

template<typename T>
vec<T> convolution_karatsuba(vec<T> a, vec<T> b)
{
    u32 n = a.size(), m = b.size();
    if (n < m) swap(n, m), a.resize(n), m += n;
    else b.resize(n), m += n;
    T *res = new T[2 * n - 1]();
    convolution_karatsuba_rec(n, a.data(), b.data(), res);
    vec<T> c(res, res + m - 1);
    delete []res;
    return c;
}

// functional format input
vec<int> solve(vec<int> a, vec<int> b)
{
    using mint = atcoder::modint998244353;
    auto t(convolution_karatsuba<mint>
        ({a.begin(), a.end()}, {b.begin(), b.end()}));
    a.resize(t.size());
    for (u32 i = 0, n = t.size(); i < n; i++) a[i] = t[i].val();
    return a;
}

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, m;
    cin >> n >> m;
    
    vec<int> a(n), b(m);
    for (auto &i: a) cin >> i;
    for (auto &i: b) cin >> i;

    a = solve(std::move(a), std::move(b));
    for (auto i: a) cout << i << ' ';
    cout << endl;
    return 0;
}