#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <array>
#include <ctime>
#include <atcoder/modint>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

// Matrix product of a and b
// For generalized matrix product such as (max, +) matrix product, use 
// Gmaxtrix_product(a, b, zero, plu, mul).
// T must have default constructor. Operator += and * must be defined on T.
template<typename T>
vec<vec<T>> matrix_product(const vec<vec<T>> &a, const vec<vec<T>> &b)
{
    u32 n = a.size(), m = b.size(), q = b[0].size();
    vec<vec<T>> res(n, vec<T>(q));
    for (u32 i = 0; i < n; i++) {
        for (u32 k = 0; k < m; k++) {
            T tmp{a[i][k]};
            for (u32 j = 0; j < q; j++) {
                res[i][j] += tmp * b[k][j];
            }
        }
    }
    return res;
}

// Generalized matrix product of a and b, 
// with user defined plus and multiplies functor, such as (+, *) <- (max, +)
// For faster (+, *) matrix product, just use matrix_product(a, b).
template<typename T, typename MU = multiplies<T>, typename PL = plus<T>>
vec<vec<T>> Gmatrix_product(const vec<vec<T>> &a, const vec<vec<T>> &b,
    const T zero = T(),
    PL plu = plus<T>(), MU mul = multiplies<T>())
{
    u32 n = a.size(), m = b.size(), q = b[0].size();
    vec<vec<T>> res(n);
    for (u32 i = 0; i < n; i++) {
        vec<T> t(q, zero);
        for (u32 k = 0; k < m; k++) {
            T tmp{a[i][k]};
            for (u32 j = 0; j < q; j++) {
                t[j] = plu(t[j], mul(tmp, b[k][j]));
            }
        }
        res[i] = t;
    }
    return res;
}

// Similar to matrix_product but has const size of matrices.
template<typename T, size_t N, size_t M, size_t Q>
array<array<T, Q>, N> matrix_product(
    const array<array<T, M>, N> &a, 
    const array<array<T, Q>, M> &b)
{
    array<array<T, Q>, N> res{};
    for (u32 i = 0; i < N; i++) {
        for (u32 k = 0; k < M; k++) {
            T tmp{a[i][k]};
            for (u32 j = 0; j < Q; j++) {
                res[i][j] += tmp * b[k][j];
            }
        }
    }
    return res;
}

// Similar to Gmatrix_product but has const size of matrices.
template<typename T, size_t N, size_t M, size_t Q, 
    typename MU = multiplies<T>, typename PL = plus<T>>
array<array<T, Q>, N> Gmatrix_product(
    const array<array<T, M>, N> &a, const array<array<T, Q>, M> &b,
    const T zero = T(),
    PL plu = plus<T>(), MU mul = multiplies<T>())
{
    array<array<T, Q>, N> res{};
    for (u32 i = 0; i < N; i++) {
        array<T, Q> t{};
        for (u32 j = 0; j < Q; j++) t[j] = zero;
        for (u32 k = 0; k < M; k++) {
            T tmp{a[i][k]};
            for (u32 j = 0; j < Q; j++) {
                t[j] = plu(t[j], mul(tmp, b[k][j]));
            }
        }
        res[i] = t;
    }
    return res;
}

using mint = atcoder::modint998244353;

signed main() 
{
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    u32 n, m, k;
    scanf("%u %u %u", &n, &m, &k);
    vec<vec<mint>> a(n, vec<mint>(m)), b(m, vec<mint>(k));
    auto read_int = [x = u32()]()mutable{return scanf("%u", &x), x;};
    for (auto &i: a) for (auto &j: i) j = read_int();
    for (auto &i: b) for (auto &j: i) j = read_int();
    auto c = matrix_product(a, b);
    for (auto &i: c) {
        for (auto j: i) cout << j.val() << ' ';
        cout << '\n';
    }
    return 0;
}