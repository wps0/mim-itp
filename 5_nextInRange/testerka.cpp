#include <bits/stdc++.h>
#include "next.h"
#include <stdint.h>

using namespace std;

#define N int(1e2)
#define LB -10000
#define UB 10000
#define TESTS_AMOUNT int(100)
#define Q 10000

struct XShift {
/*  Written in 2019 by David Blackman and Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

/* This is xoshiro128++ 1.0, one of our 32-bit all-purpose, rock-solid
   generators. It has excellent speed, a state size (128 bits) that is
   large enough for mild parallelism, and it passes all tests we are aware
   of.

   For generating just single-precision (i.e., 32-bit) floating-point
   numbers, xoshiro128+ is even faster.

   The state must be seeded so that it is not everywhere zero. */


    static inline uint32_t rotl(const uint32_t x, int k) {
        return (x << k) | (x >> (32 - k));
    }


    uint32_t s[4];

    uint32_t next(void) {
        const uint32_t result = rotl(s[0] + s[3], 7) + s[0];

        const uint32_t t = s[1] << 9;

        s[2] ^= s[0];
        s[3] ^= s[1];
        s[1] ^= s[2];
        s[0] ^= s[3];

        s[2] ^= t;

        s[3] = rotl(s[3], 11);

        return result;
    }


/* This is the jump function for the generator. It is equivalent
   to 2^64 calls to next(); it can be used to generate 2^64
   non-overlapping subsequences for parallel computations. */

    void jump(void) {
        static const uint32_t JUMP[] = { 0x8764000b, 0xf542d2d3, 0x6fa035c3, 0x77f2db5b };

        uint32_t s0 = 0;
        uint32_t s1 = 0;
        uint32_t s2 = 0;
        uint32_t s3 = 0;
        for(int i = 0; i < (int) (sizeof JUMP / sizeof *JUMP); i++)
            for(int b = 0; b < 32; b++) {
                if (JUMP[i] & UINT32_C(1) << b) {
                    s0 ^= s[0];
                    s1 ^= s[1];
                    s2 ^= s[2];
                    s3 ^= s[3];
                }
                next();
            }

        s[0] = s0;
        s[1] = s1;
        s[2] = s2;
        s[3] = s3;
    }


/* This is the long-jump function for the generator. It is equivalent to
   2^96 calls to next(); it can be used to generate 2^32 starting points,
   from each of which jump() will generate 2^32 non-overlapping
   subsequences for parallel distributed computations. */

    void long_jump(void) {
        static const uint32_t LONG_JUMP[] = { 0xb523952e, 0x0b6f099f, 0xccf5a0ef, 0x1c580662 };

        uint32_t s0 = 0;
        uint32_t s1 = 0;
        uint32_t s2 = 0;
        uint32_t s3 = 0;
        for(int i = 0; i < int(sizeof LONG_JUMP / sizeof *LONG_JUMP); i++)
            for(int b = 0; b < 32; b++) {
                if (LONG_JUMP[i] & UINT32_C(1) << b) {
                    s0 ^= s[0];
                    s1 ^= s[1];
                    s2 ^= s[2];
                    s3 ^= s[3];
                }
                next();
            }

        s[0] = s0;
        s[1] = s1;
        s[2] = s2;
        s[3] = s3;
    }
};

XShift rng;

int ask_brute(const vector<int> &x, int i, int a, int b) {
    for (int j = i; j < (int)x.size(); ++j) {
        if (a <= x[(size_t)j] && x[(size_t)j] <= b)
            return j;
    }
    return -1;
}

inline int get_i(int lb, int ub) {
    long long l = lb, u = ub;
    return int(l + ((long long)rng.next() % (u - l + 1)));
}

vector<int> gen_array(int n, int lb, int ub) {
    vector<int> arr;
    for (int i = 0; i < n; ++i) {
        arr.push_back(get_i(lb, ub));
    }
    return arr;
}

bool test_runner(const vector<int> &x, int q, int lb, int ub) {
    init(x);
    for (int i = 0; i < q; ++i) {
        int idx = get_i(0, int(x.size()-1)), a = get_i(lb, ub), b = get_i(a, ub);
        int wzor = ask_brute(x, idx, a, b);
        int rozw = nextInRange(idx, a, b);
        if (wzor != rozw) {
            printf("WA idx=%d a=%d b=%d x={", idx, a, b);
            for (int xval : x){
                printf("%d,", xval);
            }
            printf("}\n");
            printf("EXPECTED: %d\n", wzor);
            printf("GOT: %d\n", rozw);
            return false;
        }
    }
    done();
    return true;
}

int main() {
    rng.s[0] = 2137;
    rng.s[1] = 1337;
    for (int i = 0; i < TESTS_AMOUNT; ++i) {
        const vector<int> x = gen_array(N, LB, UB);
        bool res = test_runner(x, Q, LB, UB);
        if (!res) {
            return -1;
        }
    }
    printf("AC! %d tests passed!\n", TESTS_AMOUNT);
}