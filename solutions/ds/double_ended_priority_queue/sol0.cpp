#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <set>
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
    u32 n, q;
    cin >> n >> q;
    vec<int> si(n);
    for (auto &i: si) cin >> i;
    sort(si.begin(), si.end());
    multiset<int> s(si.begin(), si.end());
    int op, x;
    while (q--) {
        cin >> op;
        if (op == 0) {
            cin >> x;
            s.insert(x);
        }
        else if (op == 1) {
            cout << *s.begin() << '\n';
            s.erase(s.begin());
        }
        else {
            cout << *s.rbegin() << '\n';
            s.erase(--s.end());
        }
    }
    return 0;
}