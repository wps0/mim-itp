//==============
// Problem: 
// URL: 
//==============
#pragma GCC optimize("unroll-loops,O3")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include <bits/stdc++.h>

using namespace std;

#define MOD 1000000007
#define pb push_back
#define ppb pop_back
#define fi first
#define se second
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
typedef pair<int, bool> pib;
typedef pair<pii, int> p3i;

#define INF INT_MAX

//  ----------
//  SOLUTION
//  ----------

#define MIN_N 4
#define MAX_N 7

#define MIN_M 3
#define MAX_M 7

#define MIN_K 1
#define MAX_K 7

mt19937 rng;

inline int gi(int from, int to) {
	return uniform_int_distribution<int>(from, to)(rng);
}

inline ll f(ll x) {
	return (ll)(1337 * x % MOD * x % MOD * x % MOD
		+ 29*x % MOD * x % MOD
		+ 2*x % MOD
		+ 11) % MOD;
}

void init() {
	int seed;
	scanf("%d", &seed);
	rng = mt19937(f(seed));
}

vector<vector<char>> board;
int _n, _m, _k;


vector<vector<char>> resize(int n, int m) {
	vector<vector<char>> v;
	v.resize(n);
	REP(i, n) v[i].resize(m, '.');
	return v;
}

vector<vector<char>> gen_pattern(char letter, int max_size, bool random = true, bool apply = true) {
	int sz = gi(min(2, max_size), max_size);
	vector<vector<char>> pat = resize(_n, _m);

	if (random) {
		for (int i = 0; i < sz; ++i) {
			pat[gi(0, _n-1)][gi(0, _m-1)] = letter;
		}
	} else {
        bool empty = true;
		REP(i, _n) {
			if (gi(1, 5) == 1) {
				i++;
				continue;
			}
			REP(j, _m) {
				if (gi(1, 7) == 1) {
					j++;
					continue;
				}
				if (sz == 0) continue;
				if (board[i][j] == '.'){
					pat[i][j] = letter;
					sz--;
                    empty = false;
				}
			}
		}
        if (empty) {
//            printf("rng\n");
            pat = gen_pattern(letter, max_size, true, false);
        }
	}
	if (apply) {
		REP(i, _n) {
			REP(j, _m) {
                if (pat[i][j] != '.')
				    board[i][j] = pat[i][j];
			}
		}
	}

	return pat;
}

void print_pattern(vector<vector<char>> p, FILE *f = stdout) {
	REP(i, p.size()) {
		REP(j, p[i].size()) {
			fprintf(f, "%c", p[i][j]);
		}
		fprintf(f, "\n");
	}
}



int main() {
	init();
	_n = gi(MIN_N, MAX_N), _m = gi(MIN_M, MAX_M), _k = gi(MIN_K, MAX_K);
	printf("%d %d %d\n", _n, _m, _k);
	board = resize(_n, _m);

	char id ='A';

    print_pattern(gen_pattern(id++, max(1, gi(_m*_n/6, _m*_n/2))));
    for (int i = 0; i < _k-1; ++i) {
//		 bool is_rng = gi(0, 1);
        print_pattern(gen_pattern(id++, max(1, gi(_m*_n/4, _m*_n/2)), false, true));
	}
//    fprintf(stderr, "----\n");
//    print_pattern(board, stderr);
}
