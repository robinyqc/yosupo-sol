#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <array>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

template<typename M, typename STR = ostream,
typename EOL = decltype(endl<char, char_traits<char>>), typename BLK = char>
void print_clang_format_matrix(
    const M& a, 
    const string &hint = "", 
    const string &tab = "    ", 
    const EOL eol = endl<char, char_traits<char>>,
    STR &out = cout)
{
    u32 n = a.size();
    out << '{';
    if (n == 0) {
        out << ' ' << '}' << eol;
        return ;
    }
    out << eol;
    u32 m = a[0].size();
    for (u32 i = 0; i < n; i++) {
        out << tab << hint << '{';
        for (u32 j = 0; j < m - 1; j++) {
            out << a[i][j] << ", ";
        }
        out << a[i][m - 1] << '}';
        if (i == n - 1) out << eol;
        else out << ',' << eol;
    }
    out << '}' << eol;
}

signed main() 
{
    array<array<int, 3>, 3> a = {};
    a[0][0] = 2, a[1][2] = 9, a[2][0] = 8;
    a[0][1] = 3, a[0][2] = 8, a[2][2] = 1;
    vec<vec<int>> b(3, vec<int>(3));
    b[0][0] = 2, b[1][2] = 9, b[2][0] = 8;
    b[0][1] = 3, b[0][2] = 8, b[2][2] = 1;
    print_clang_format_matrix(a, "array<int, 3>");
    print_clang_format_matrix(b, "vec<int>");
    return 0;
}