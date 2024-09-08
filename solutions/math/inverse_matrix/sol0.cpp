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

#define MATRIX_VALTYPE typename M::value_type::value_type
template<typename M>
M inverse(M a, const MATRIX_VALTYPE unit = MATRIX_VALTYPE(1), 
    const MATRIX_VALTYPE zero = MATRIX_VALTYPE())
{
    u32 n = a.size();
    M res(a);
    for (u32 i = 0; i < n; i++) {
        res[i][i] = unit;
        for (u32 j = 0; j < i; j++) res[i][j] = zero;
        for (u32 j = i + 1; j < n; j++) res[i][j] = zero;
    }
    MATRIX_VALTYPE val(zero);
    for (u32 i = 0; i < n; i++) {
        for (u32 j = i + 1; j < n; j++) {
            if (a[j][i] != zero) {
                a[i].swap(a[j]);
                res[i].swap(res[j]);
                break;
            }
        }
        val = a[i][i];
        if (val == zero) return {};
        for (u32 k = i; k < n; k++) a[i][k] = a[i][k] / val;
        for (u32 k = 0; k < n; k++) res[i][k] = res[i][k] / val;
        for (u32 j = 0; j < n; j++) {
            if (j != i && a[j][i] != zero) {
                val = a[j][i];
                for (u32 k = i; k < n; k++) a[j][k] -= a[i][k] * val;
                for (u32 k = 0; k < n; k++) res[j][k] -= res[i][k] * val;
            }
        }
    }
    return res;
}
#undef MATRIX_VALTYPE

using mint = atcoder::modint998244353;

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, x;
    cin >> n;
    vec<vec<mint>> a(n, vec<mint>(n));
    for (auto &i: a) for (auto &j: i) {
        cin >> x;
        j = x;
    }
    a = inverse(std::move(a));
    if (a.size() == 0) {
        cout << "-1\n";
    }
    else {
        for (auto &i: a) {
            for (auto j: i) cout << j.val() << ' ';
            cout << '\n';
        }
    }
    return 0;
}