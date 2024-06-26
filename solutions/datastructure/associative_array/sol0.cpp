#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <map>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

template<typename T> using vec = vector<T>;

signed main() 
{
    int n;
    cin >> n;
    map<i64, i64> a;
    i64 k, v;
    for (int i = 0; i < n; i++) {
        cin >> k;
        if (k == 0) {
            cin >> k >> v;
            a[k] = v;
        }
        else {
            cin >> k;
            cout << a[k] << '\n';
        }
    }
    return 0;
}