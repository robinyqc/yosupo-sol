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
using mint = atcoder::modint998244353;

// Basic method to calculate determinant. 
// But it's worse than determinant(x). So use determinant(x) instead.
template<typename DynMint>
DynMint determinant_on_field(vec<vec<DynMint>> a)
{
    u32 n = a.size(), w = 0;
    DynMint res = DynMint::raw(1);
    for (u32 i = 0; i < n; i++) {
        for (u32 j = i + 1; j < n; j++) {
            if (a[j][i] != 0) {
                a[i].swap(a[j]);
                w ^= 1u;
                break;
            }
        }
        DynMint val = a[i][i];
        if (val == 0) return DynMint::raw(0);
        for (u32 j = i + 1; j < n; j++) {
            if (a[j][i] != 0) {
                DynMint dec = a[j][i] / val;
                for (u32 k = i; k < n; k++) {
                    a[j][k] -= dec * a[i][k];
                }
            }
        }
        res *= a[i][i];
    }
    if (w) return -res;
    return res;
}

#define MATRIX_VALTYPE typename M::value_type::value_type
template<typename M>
MATRIX_VALTYPE determinant(M a)
{
    u32 n = a.size(), w = 0;
    MATRIX_VALTYPE res(1);
    for (u32 i = 0; i < n; i++) {
        for (u32 j = i + 1; j < n; j++) {
            while (a[j][i] != 0) {
                if (a[i][i] != 0) {
                    MATRIX_VALTYPE quot(a[j][i].val() / a[i][i].val());
                    for (u32 k = i; k < n; k++) {
                        a[j][k] -= a[i][k] * quot;
                    }
                }
                w ^= 1u;
                a[i].swap(a[j]);
            }
        }
        res *= a[i][i];
    }
    if (w) return -res;
    return res;
}
#undef MATRIX_VALTYPE

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n;
    cin >> n;
    vec<vec<mint>> a(n, vec<mint>(n));
    for (auto &i: a) for (auto &j: i) {
        u32 x;
        cin >> x;
        j = x;
    }
    cout << determinant(std::move(a)).val() << endl;
    return 0;
}