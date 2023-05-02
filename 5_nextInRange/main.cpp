#include <bits/stdc++.h>
using namespace std;

#include "next.h"

mt19937 rng;
int ri(int a, int b) {
    return uniform_int_distribution<int>(a, b)(rng);
}

void test1() {
    printf("Test#1: \n");
    const vector<int> x{5,3,-5,10};
    init(x);

    // test1.1
    assert(nextInRange(3, 10, 10) == 3);
    assert(nextInRange(3, -10, -10) == -1);
    assert(nextInRange(3, 11, 11) == -1);
    assert(nextInRange(3, INT_MIN, INT_MAX) == 3);
    assert(nextInRange(3, 2, 10) == 3);
    // test1.2
    assert(nextInRange(2, 0, 10) == 3);
    assert(nextInRange(2, -10, -5) == 2);
    assert(nextInRange(2, 11, 11) == -1);
    assert(nextInRange(2, INT_MIN, INT_MAX) == 2);
    assert(nextInRange(2, 2, 8) == -1);
    // test1.3
    assert(nextInRange(1, 0, 10) == 1);
    assert(nextInRange(1, -10, -5) == 2);
    assert(nextInRange(1, 11, 11) == -1);
    assert(nextInRange(1, INT_MIN, INT_MAX) == 1);
    assert(nextInRange(1, 4, 10) == 3);
    // test1.4
    assert(nextInRange(0, 5, 10) == 0);
    assert(nextInRange(0, 6, 10) == 3);
    assert(nextInRange(0, 0, 4) == 1);
    assert(nextInRange(0, -5, 2) == 2);
    assert(nextInRange(0, 6, 6) == -1);
    assert(nextInRange(0, INT_MIN, INT_MAX) == 0);
    assert(nextInRange(0, 2, 8) == 0);
    assert(nextInRange(0, 5, 5) == 0);
    assert(nextInRange(0, 3, 3) == 1);
    // test1.5
    assert(nextInRange(1, 3, 3) == 1);
    assert(nextInRange(3, 3, 3) == -1);
    assert(nextInRange(0, -10, 3) == 1);

    // cleanup
    done();

    printf("OK\n");
}

void test2() {
    rng = mt19937(chrono::system_clock::now().time_since_epoch().count());
    const int n = int(1e4);
    const int q = int(0);
    printf("Test#2: \n");
    vector<int> x;
    for (int i = 0; i < n; ++i) {
        x.push_back(ri(INT_MIN, INT_MAX));
    }
    init(x);

    long long s = 0;
    for (int i = 0; i < q; ++i) {
        int a = ri(INT_MIN, INT_MAX-1);
        s += nextInRange(ri(0, n-1), a, ri(a, INT_MAX));
    }
    printf("%lld\n", s);
    done();
}

int main() {
//    test1();
    test2();
}