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

class Brute2 {
private:
    struct Pattern {
        char let;
        int height, width;
        // relatywne koordynaty do pola
        vector<pii> squares;
        bitset<128> squares_as_bits;

        Pattern(int _height, int _width) : let('.'), height(_height), width(_width) {}

        void read_squares() {
            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    char sq;
                    assert(scanf(" %c", &sq));
                    if (sq == '.')
                        continue;
                    let = sq;
                    squares.emplace_back(i, j);
                }
            }
        }

        void normalise_squares() {
            int y_rel = height, x_rel = width;
            for (int i = 0; i < squares.size(); ++i) {
                y_rel = min(y_rel, squares[i].first);
                x_rel = min(x_rel, squares[i].second);
            }
            for (pii &sq : squares) {
                sq.first -= y_rel;
                sq.second -= x_rel;
                flip_bit(sq.first, sq.second);
            }
            sort(squares.begin(), squares.end());
        }

        // wywoływać przed normalise_squares!
        static Pattern rotate(Pattern r) {
            for (int i = 0; i < r.squares.size(); ++i) {
                int x = r.squares[i].first, y = r.squares[i].second;
                r.squares[i] = {r.width - y - 1, x};
            }
            swap(r.width, r.height);
            return r;
        }

        bool operator==(const Pattern &rhs) {
            return squares == rhs.squares;
        }

        bool operator!=(const Pattern &rhs) {
            return !(*this == rhs);
        }

        int size() {
            return squares.size();
        }

        void flip_bit(int y, int x) {
            squares_as_bits.flip(y*height + x);
        }

        void print_coloured() {
            printf("id=%c\n", let);
            for (pii p : squares) {
                printf("<%d, %d>, ", p.first, p.second);
            }
            printf("\n");
        }
        void print() {
            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    if (find(squares.begin(), squares.end(), make_pair(i, j)) != squares.end())
                        printf("%c", let);
                    else
                        printf(".");
                }
                printf("\n");
            }
        }
    };

    struct Blueprint {
        // 0 - basicowy
        // k - przekręcony o k*90 stopni
        vector<Pattern> rotated;

        void init(int height, int width) {
            Pattern b(height, width);
            b.read_squares();

            rotated.push_back(b);
            for (int i = 0; i < 3; ++i) {
                rotated.push_back(Pattern::rotate(*rotated.rbegin()));
            }
            for (Pattern &block : rotated) {
    //            printf("-- znormalizowana:\n");
                block.normalise_squares();
    //            block.print();
            }
            remove_duplicates();
        }

        void remove_duplicates() {
            const vector<Pattern> old = rotated;
            rotated.clear();
            for (const Pattern &b : old) {
                auto it = rotated.begin();
                while ((it = find(rotated.begin(), rotated.end(), b)) != rotated.end()) {
                    rotated.erase(it);
                }
                rotated.push_back(b);
            }
        }
    };

    struct Board {
        int height, width;
        // plansza
        vector<vector<char>> board;
        bitset<128> board_as_bits;
        // ile już pokrytych
        int coverage;

        Board(int height, int width) : height(height), width(width) {
            coverage = 0;
            board.resize(height);
            for (int i = 0; i < height; ++i) {
                board[i].resize(width, '.');
            }
        }

        bool can_place(int offset_x, int offset_y, const Pattern &block) {
            for (pii p : block.squares) {
                int y = p.first + offset_y, x = p.second + offset_x;
                if (min(x, y) < 0 || x >= width || y >= height)
                    return false;
                if (board[y][x] != '.')
                    return false;
            }
            return true;
        }

        bool place(int offset_y, int offset_x, const Pattern &block) {
            if (!can_place(offset_x, offset_y, block))
                return false;
            for (pii p : block.squares) {
                int y = p.first + offset_y, x = p.second + offset_x;
                board[y][x] = block.let;
            }
            coverage += block.squares.size();
            return true;
        }

        void undo_place(int offset_y, int offset_x, const Pattern &block) {
            for (pii p : block.squares) {
                int y = p.first + offset_y, x = p.second + offset_x;
                assert(board[y][x] == block.let);
                board[y][x] = '.';
            }
            coverage -= block.squares.size();
        }

        inline bool is_fully_covered() {
            return coverage == height*width;
        }

        void print() {
            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    printf("%c", board[i][j]);
                }
                printf("\n");
            }
        }
    };

    /////////////
    int n, m, k;
    vector<Blueprint> bl;
    vector<bool> used;

    void init() {
        n = m = k = 0;
        assert(scanf("%d%d%d", &n, &m, &k));
        bl.resize(k);
        used.resize(k, false);
        for (int i = 0; i < k; ++i) {
            bl[i].init(n, m);
        }
        sort(bl.begin(), bl.end(), [](Blueprint b1, Blueprint b2) {
            return b1.rotated.rbegin()->size() >= b2.rotated.rbegin()->size();
        });
    }

    bool backtrack(int offset_i, Board &b) {
        if (b.is_fully_covered())
            return true;
        for (int i = offset_i; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                for (int pat_id = 0; pat_id < k; ++pat_id) {
                    if (used[pat_id])
                        continue;
                    for (const Pattern &variant : bl[pat_id].rotated) {
                        if (b.place(i, j, variant)){
                            used[pat_id] = true;
                            if (backtrack(i, b))
                                return true;
                            b.undo_place(i, j, variant);
                            used[pat_id] = false;
                        }
                    }
                }
            }
        }
        return false;
    }
public:
    int run() {
        init();
        Board b(n, m);
        return backtrack(0, b);
    }
};



void print(const vector<vector<char>> &v) {
    REP(i, v.size()) {
        REP(j, v[i].size()) {
            fprintf(stderr, "%c", v[i][j]);
        }
        fprintf(stderr, "\n");
    }
}

class Sprawdzarka {


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
        void read_from_string_by_letter(char letter, const vector<string> &in) {
            let = letter;
            REP(i, height) REP(j, width) {
                    char c = in[i][j];
                    if (c == letter) {
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

        inline bool operator==(const Pattern &rhs) const {
            return b == rhs.b;
        }
        inline bool operator!=(const Pattern &rhs) const {
            return !(b == rhs.b);
        }

        void print_board() {
            vector<vector<char>> board;
            board.resize(height);
            REP(i, height) board[i].resize(width, '.');
            for (pii x : b) board[x.fi][x.se] = let;
            print(board);
        }
    };



public:
    int n, m, k;
    vector<vector<Pattern>> patterns;

    void init() {
        assert(scanf("%d%d%d", &n, &m, &k));
        patterns.resize(k);
        for (int i = 0; i < k; ++i) {
            Pattern p(char('A'+i), n, m);
            p.read();
            p.align();
            patterns[i].pb(p);
            for (int j = 0; j < 3; ++j) {
                p = p.rotate();
                p.align();
                patterns[i].pb(p);
            }
        }
    }

    bool is_prog_output_valid(vector<string> &o) {
        REP(i, n) REP(j, m) {
                if (o[i][j] < 'A' || o[i][j] > 'A'+k-1)
                    return false;
            }
        REP(i, k) {
            char let = 'A'+k;
            Pattern p(let, n, m);
            p.read_from_string_by_letter(let, o);
            p.align();
            if (p.b.empty())
                continue;
            bool match = false;
            for (const Pattern &cand : patterns[i]) {
                if (cand == p)
                    match = true;
            }
            if (!match) {
                printf("Pattern '%c' does not match\n", let);
                return false;
            }
        }
        return true;
    }
};



///// WEJŚCIE:
// wejście do sprawdzarki
// wejście do brute
int main() {
    Brute2 bf;
    string expected_ans = bf.run() ? "TAK" : "NIE";

    Sprawdzarka spr;
    spr.init();
    string prog_ans;
    cin >> prog_ans;

    if (expected_ans != prog_ans) {
        printf("WA:\n  expected %s\n  got %s\n", expected_ans.c_str(), prog_ans.c_str());
    } else {
        if (expected_ans == "NIE") {
            printf("OK");
            return 0;
        }
        vector<string> prog_board;
        REP(i, spr.n) {
            string s;
            cin >> s;
            prog_board.pb(s);
        }
        if (spr.is_prog_output_valid(prog_board)) {
            printf("OK");
        } else {
            printf("WA: Invalid output\n");
        }
    }
}