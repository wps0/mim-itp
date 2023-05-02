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


int n, m, k;

void print(const vector<vector<char>> &v) {
        REP(i, v.size()) {
            REP(j, v[i].size()) {
                fprintf(stderr, "%c", v[i][j]);
            }
            fprintf(stderr, "\n");
        }
}



struct Pattern {
    char let;
    int height, width;
    vector<pii> b;

    Pattern(char let, int height, int width) : let(let), height(height), width(width) {}

    void read() {
        REP(i, height) REP(j, width) {
            char c;
            scanf(" %c", &c);
            if (c != '.') {
                let = c;
                b.pb({i, j});
            }
        }
    }

    Pattern rotate() {
        Pattern rot(let, width, height);
        for (pii x : b) {
            rot.b.emplace_back(width-x.second-1, x.first);
//            printf("<%d, %d> => <%d, %d>\n", x.fi, x.se, rot.b.rbegin()->fi, rot.b.rbegin()->se);
        }
        return rot;
    }

    void align() {
        int al_i = 1337, al_j = 1337;
        for (pii x : b) {
            al_i = min(x.fi, al_i);
            al_j = min(x.se, al_j);
        }
        for (pii &x : b) {
            x.fi -= al_i;
            x.se -= al_j;
        }
    }

    void print_board() {
        vector<vector<char>> board;
        board.resize(height);
        REP(i, height) board[i].resize(width, '.');
        for (pii x : b) board[x.fi][x.se] = let;
        print(board);
    }
};

struct Board {
    vector<vector<char>> b;
    int coverage;
    void init() {
        coverage = 0;
        b.resize(n);
        REP(i, n) b[i].resize(m, '.');
    }

    bool can_place(int i, int j, const Pattern &p) {
        for (pii x : p.b) {
            int i2 = x.fi + i, j2 = x.se + j;
            if (i2 >= n || j2 >= m)
                return false;
            if (b[i2][j2] != '.')
                return false;
        }
        return true;
    }

    void place(int i, int j, const Pattern &p) {
        if (!can_place(i, j, p))
            return;
        for (pii x : p.b) {
            b[x.fi+i][x.se+j] = p.let;
        }
        coverage += p.b.size();
    }

    void undo_place(int i, int j, const Pattern &p) {
        for (pii x : p.b) {
            b[x.fi+i][x.se+j] = '.';
        }
        coverage -= p.b.size();
    }

    bool is_fully_covered() {
        return n*m == coverage;
    }
};


//////////////
vector<vector<Pattern>> patterns;
Board brd;

bool brute(int idx) {
//    printf("idx=%d\n", idx);
//    print(brd.b);
    if (idx == k || brd.is_fully_covered()) {
//        printf("===\n");
        return brd.is_fully_covered();
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            for (int p = 0; p < patterns[idx].size(); ++p) {
                if (brd.can_place(i, j, patterns[idx][p])) {
                    brd.place(i, j, patterns[idx][p]);
                    if (brute(idx+1))
                        return true;
                    brd.undo_place(i, j, patterns[idx][p]);
                }
            }
            if (brute(idx+1)) {
                return true;
            }
        }
    }
    return false;
}

void init() {
    scanf("%d%d%d", &n, &m, &k);
    brd.init();
    patterns.resize(k);
    for (int i = 0; i < k; ++i) {
        Pattern p(char('A'+i), n, m);
        p.read();
        p.align();
//        p.print_board();
        patterns[i].pb(p);
        for (int j = 0; j < 3; ++j) {
            p = p.rotate();
            p.align();
            patterns[i].pb(p);
        }
    }
}

int main() {
    init();

    if (brute(0)) {
        printf("TAK\n");
    } else {
        printf("NIE\n");
    }
}