#include <bits/stdc++.h>

using namespace std;

#define MOD 1000000007
#define pb push_back
#define ppb pop_back
#define fi first
#define se second
#define mp(x, y) make_pair((x), (y))
#define REP(i, n) for (int (i) = 0; (i) < (n); (i)++)
#define REPE(i, n) for (int (i) = 0; (i) <= (n); (i)++)
#define FOR(i, z, n) for (int (i) = (z); (i) < (n); (i)++)
#define FORE(i, z, n) for (int (i) = (z); (i) <= (n); (i)++)

typedef unsigned int uint;
typedef unsigned long long ull;
typedef long long ll;
typedef pair<ll, ll> pll;
typedef pair<int, int> pii;
typedef pair<bool, int> pbi;
typedef pair<pii, int> p3i;

#define INF INT_MAX

//  ----------
//  GENERATORKA
//  ----------
mt19937 rng;
#define MIN_N 1000000
#define MAX_N 1000000
	
#define MIN_M 1
#define MAX_M 10
#define MIN_D 0
#define MAX_D 100


inline ll f(ll x){
	return ((((3*((((x*x)%MOD)*x)%MOD))%MOD
		+ (2137*((x*x)%MOD))%MOD)%MOD)
		+ 1337)%MOD;
}
inline int gi(int st, int kon) {
	return uniform_int_distribution<int>(st, kon)(rng);
}

void init() {
	int seed;
	scanf("%d", &seed);
	rng = mt19937(f(seed));
}

int main() {
	init();
    int n = gi(MIN_N, MAX_N), m = gi(MIN_M, MAX_M);
    ll dis = 0;
    printf("%d\n", n);
    REP(i, n) {
        int col = gi(1, m);
        dis += gi(MIN_D, MAX_D);
        printf("%d %lld\n", col, dis);
    }
}
