#include <cstdio>
#include <vector>
#include <cassert>
#include <algorithm>
#include <bitset>
#include <iostream>

using namespace std;

typedef pair<int, int> pii;

#define REAL_BOARD_SIZE (10*10)
// w celu uproszczenia implementacji trzymam podwojony board size
#define BOARD_SIZE (2*REAL_BOARD_SIZE)
// ile razy obracać patterny o 90 stopni
const int ROTATIONS = 3;

/////////
// BoardState trzyma pewną konfigurację planszy
// Operacje na stanie działają w O(n*m/32)
struct BoardState {
private:
    bitset<BOARD_SIZE> out_of_bounds_mask;
public:
    // wysokość, szerokość; tak na prawdę zawsze 10x10 - prostsza implementacja
    int hei, wid;
    bitset<BOARD_SIZE> state;

    BoardState(int height, int width) {
        state.reset();

        out_of_bounds_mask.set();
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                out_of_bounds_mask[i*10+j] = 0;
            }
        }
        hei = 10, wid = 10;
    }

    // zwraca true jeśli można zaaplikować stan s w taki sposób, żeby punkt (0,0) ze stanu s
    //  znanazł się na (off_h, off_w) tego stanu
    // TODO: tu jest błąd. Np. dla testu poniżej odpowiedź to NIE, a zwraca TAK (test 18290):
    /* 4 7 7
.......
A.A.A..
A.....A
......A

.......
.......
.BBBBB.
BB.....

CC.....
.......
.......
.......

......D
.....D.
.....D.
.......

..EEEE.
.......
.......
..E..E.

.......
.F....F
.......
.......

.......
.......
.......
...GG..
     */
    // Tu nie sprawdzam, czy nie shiftuję do następnego rzędu
    bool can_apply_state(int off_h, int off_w, BoardState s) {
        s.move_0_0_to(off_h, off_w);
        // pattern wychodzi poza planszę
        if ((out_of_bounds_mask & s.state).any())
            return false;
        // stan s i stan state nie nachodzą się
        return (state & s.state).none();
    }

    void apply_state(int off_h, int off_w, BoardState s) {
        assert(can_apply_state(off_h, off_w, s));
        s.move_0_0_to(off_h, off_w);
        state |= s.state;
    }

    // założenie: stan s był wcześniej zaaplikowany do tego stanu i s jest alignowany do (0,0)
    void undo_state(int off_h, int off_w, BoardState s) {
        s.move_0_0_to(off_h, off_w);
        state ^= s.state;
    }

    // przesuwa lewy górny róg patternu z punktu (0,0) do (off_h, off_w)
    inline void move_0_0_to(int off_h, int off_w) {
        state <<= i_from_pos(off_h, off_w);
    }

    // przesuwa komórki jak najbardziej do lewej i do góry: tak żeby nie było pustego miejsca
    void align() {
        // maska do sprawdzania czy są zapalone bity w rzędzie
        bitset<BOARD_SIZE> mask_row = (1<<wid)-1;
        // maska do sprawdzania czy są zapalone bity w kolumnie
        bitset<BOARD_SIZE> mask_col;
        for (int i = 0; i < hei; ++i) {
            mask_col.set(i_from_pos(i, 0));
        }
        int hei_off = 0, wid_off = 0;
        for (int i = 0; i < hei; ++i) {
            if ((state & mask_row).any())
                break;
            hei_off++;
            mask_row <<= wid;
        }
        for (int i = 0; i < wid; ++i) {
            if ((state & mask_col).any())
                break;
            wid_off++;
            mask_col <<= 1;
        }
        state >>= wid*hei_off + wid_off;
    }

    inline bool at(int i, int j) const {
        return state[i_from_pos(i, j)];
    }
    inline void set(int i, int j, bool val) {
        state[i_from_pos(i, j)] = val;
    }
    inline size_t i_from_pos(int i, int j) const {
        return size_t(i*wid + j);
    }

    inline bool operator==(const BoardState &rhs) const {
        return state == rhs.state;
    }
    inline bool operator!=(const BoardState &rhs) const {
        return !(state == rhs.state);
    }

    void print(char let) const {
        for (int i = 0; i < hei; ++i) {
            for (int j = 0; j < wid; ++j) {
                printf("%c", at(i, j) ? let : '.');
            }
            printf("\n");
        }
    }

    static BoardState rotate(const BoardState &bs) {
        BoardState transformed(bs.wid, bs.hei);
        for (int i = 0; i < bs.hei; ++i) {
            for (int j = 0; j < bs.wid; ++j) {
                if (!bs.at(i, j))
                    continue;
                transformed.set(bs.wid - j - 1, i, true);
            }
        }
        return transformed;
    }
};

// Trzyma pattern z wejścia
struct Pattern {
    // litera przypisana do patternu
    char let;
    // warianty patternu (tj. obrócenie)
    vector<BoardState> variants;
    int size;

    Pattern() : size(0) {}

    void read_and_init_pattern(int hei, int wid) {
        BoardState basic_var = read_variant(hei, wid);
        basic_var.align();
        size = int(basic_var.state.count());

        variants.push_back(basic_var);
        for (int i = 0; i < ROTATIONS; ++i) {
            BoardState rot_var = BoardState::rotate(*variants.rbegin());
            rot_var.align();
            // pomijam duplikaty
            auto it = find(variants.begin(), variants.end(), rot_var);
            if (it != variants.end()){
                break; // rotacje zaczynają się zacyklać
            }
            variants.push_back(rot_var);
        }
    }

    // wczytuje BoardState z stdin
    BoardState read_variant(int hei, int wid) {
        BoardState p(hei, wid);
        for (int i = 0; i < hei; ++i) {
            for (int j = 0; j < wid; ++j) {
                char sq;
                assert(scanf(" %c", &sq));
                if (sq == '.')
                    continue;
                let = sq;
                p.set(i, j, true);
            }
        }
        return p;
    }
};

/////////////
int n, m, k;
vector<Pattern> patterns;

// plansza do backtrackingu
BoardState board = BoardState(0, 0);
// maska bitowa reprezentująca użyte patterny - do backtrackingu
int used;
// dp[i] - i to suma pokrytych pól. Maska bitowa dp[i] oznacza, które elementy występują w jakiejkolwiek sumie i.
bitset<11> dp[REAL_BOARD_SIZE+1];
// ans to pokrycie całej planszy albo pusty wektor - gdy takie nie istnieje
vector<vector<char>> ans;

void update_ans(char letter, const BoardState p) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (p.at(i, j)) {
                assert(ans[i][j] == '.');
                ans[i][j] = letter;
            }
        }
    }
}

void print_ans() {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            printf("%c", ans[i][j]);
        }
        printf("\n");
    }
}


void calc_dp() {
    dp[0].set(10);
    for (int i = 0; i < k; ++i) {
        int p_size = patterns[i].size;
        for (int j = REAL_BOARD_SIZE; j >= p_size; --j) {
            if (dp[j - p_size].any()) {
                dp[j] |= dp[j - p_size];
                dp[j].set(i);
            }
        }
    }
}

// wczytuje input, inicjalizuje zmienne
void init() {
    assert(scanf("%d%d%d", &n, &m, &k));
    board = BoardState(n, m);
    for (int i = 0; i < k; ++i) {
        Pattern p;
        p.read_and_init_pattern(n, m);
        patterns.push_back(p);
    }

    // sortuję patterny po rozmiarze, żeby rozpatrywać je później od największego do najmniejszego;
    //  czasami przyspiesza
    sort(patterns.begin(), patterns.end(), [](const Pattern &lhs, const Pattern &rhs){
        return lhs.size >= rhs.size;
    });

    used = 0;
    ans.resize(n);
    for (int i = 0; i < n; ++i) {
        ans[i].resize(m, '.');
    }
}

// i, j - górny róg, od którego zacząć
// sum_remaining - ilość niepokrytych pól
// zwraca true jeśli udało się znaleźć pokrycie, a wpp. false
bool backtracking(int i, int j, int sum_remaining) {
    if (sum_remaining == 0)
        return true;
    while (i < n) {
        while (j < m) {
            for (int pid = 0; pid < k; pid++) {
                const int exp = 1<<pid;
                if ((used & exp) || !dp[sum_remaining].test(pid))
                    continue;
                // próbuje wszystkie patterny obrócone o x stopni (w szczególności x = 0):
                for (const BoardState &v : patterns[pid].variants) {
                    if (board.can_apply_state(i, j, v)) {
                        used |= exp;
                        board.apply_state(i, j, v);

                        if (backtracking(i, j, sum_remaining-patterns[pid].size)) {
                            BoardState covered_by_v = v;
                            covered_by_v.move_0_0_to(i, j);
                            update_ans(patterns[pid].let, covered_by_v);
                            return true;
                        }

                        board.undo_state(i, j, v);
                        used ^= exp;
                    }
                }
            }
            j++;
        }
        j = 0;
        i++;
    }
    return false;
}

int main() {
    init();
    calc_dp();
  if (backtracking(0, 0, n*m)) {
        printf("TAK\n");
        print_ans();
    } else {
        printf("NIE\n");
    }
}