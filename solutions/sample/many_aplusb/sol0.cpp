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

signed main() 
{
    ios::sync_with_stdio(false); 
    cin.tie(0), cout.tie(0);
    u32 t, a, b;
    cin >> t;
    while (t--) {
        cin >> a >> b;
        cout << a + b << '\n';
    }
    return 0;
}