#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <atcoder/modint>

template<int m>
std::istream &operator>>(std::istream &in, atcoder::static_modint<m> &x)
{
    static int val;
    in >> val;
    x = val;
    return in;
}

template<int m>
std::ostream &operator<<(std::ostream &out, atcoder::static_modint<m> x)
{
    return out << x.val();
}

template<typename T>
std::istream &operator>>(std::istream &in, std::vector<T> &x)
{
    for (auto &i: x) in >> i;
    return in;
}

template<typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &x)
{
    out << "{";
    for (std::size_t i = 0, j = x.size(); i + 1 < j; i++) {
        out << x[i] << ", ";
    }
    out << x.back() << "}";
    return out;
}

signed main() 
{
    return 0;
}