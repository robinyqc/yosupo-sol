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
    int n, q;
    cin >> n >> q;
    string t;
    cin >> t;
    vec<int> a;
    a.reserve(n);
    for (int i = 0; i < n; i++) if (t[i] == '1') a.emplace_back(i);
    set<int> s(a.begin(), a.end());
    int op, k;
    while (q--) {
        cin >> op >> k;
        if (op == 0) s.emplace(k);
        else if (op == 1) {
            auto it = s.find(k);
            if (it != s.end()) s.erase(it);
        }
        else if (op == 2) {
            cout << s.count(k) << '\n';
        }
        else if (op == 3) {
            auto it = s.lower_bound(k);
            if (it != s.end()) cout << *it << '\n';
            else cout << "-1\n";
        }
        else {
            auto it = s.upper_bound(k);
            if (it != s.begin()) cout << *--it << '\n';
            else cout << "-1\n";
        }
    }
    return 0;
}