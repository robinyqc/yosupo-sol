#include <iostream>
#include <ctime>
#include <unistd.h>
#include <chrono>
using namespace std;

using i32 = int;
using i64 = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

signed main(int argc, char **argv) 
{
    system("touch sol0.cpp; make sol0");
    system("touch sol1.cpp; make sol1");
    int t = 0, ol = 0, flt = 256;
    if (argc > 1) ol = stoi(argv[1]);
    if (argc > 2) flt = stoi(argv[2]);
    do {
        if (system("python gen.py > a.in")) return 0;
        if (system("./sol0 < a.in > a.out")) break;
        if (system("./sol1 < a.in > a.ans")) break;
        t++;
        cout << t << endl;
        if (t % flt == 0) system("clear");
        if (t == ol) {
            cout << "AC in " << t << " Case(s)" << endl;
            return 0;
        }
    }
    while (!system("diff a.out a.ans -w -q"));
    cout << "WA, RE, or TLE." << endl;
    return 0;
}