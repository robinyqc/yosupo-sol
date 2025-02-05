#include <cmath>

unsigned long long floor_sqrt(unsigned long long x)
{
    static unsigned long long y;
    y = std::sqrt(x);
    while (y * y < x) ++y;
    while (y * y > x) --y;
    return y;
}