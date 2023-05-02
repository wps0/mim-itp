// Pomysł: dla każdej trójki moteli (u, v, w) iterować się po v i liczyć odległości do najdalszych/najbliższych u oraz w.
//  Typy niektórych u i w mogą się pokrywać, dlatego dla każdego v trzymam po 3 różne u i 3 różne w.
#include <assert.h>
#include <stdio.h>
#include <malloc.h>
#include <limits.h>
#include <stdbool.h>

#define INF (LLONG_MAX/2LL)
#define M_PUSTY ((M){.col = -1, .pos = -1})

typedef long long ll;
typedef struct Motel{
    int col; // color
    ll pos;
} M;

int _n;
M *_arr;
// trzy polozone przed i-tym o roznych kolorach
M **m_przed;
// trzy polozone za i-tym o roznych kolorach
M **m_za;

inline bool is_empty(M motel) {
    return motel.pos < 0 || motel.col < 0;
}
inline ll min(ll a, ll b) {
    return a > b ? b : a;
}
inline ll max(ll a, ll b) {
    return a < b ? b : a;
}

// Znajduje indeks, na którym jest max/min pod względem pos wśród m_za[i] lub m_przed[i]
int find_min(M *m) {
    int mini = 0;
    for (int i = 1; i < 3; ++i) {
        if (m[mini].pos > m[i].pos)
            mini = i;
    }
    return mini;
}
int find_max(M *m) {
    int maxi = 0;
    for (int i = 1; i < 3; ++i) {
        if (m[maxi].pos < m[i].pos)
            maxi = i;
    }
    return maxi;
}

// preprocessing - znajdowanie 3 najbliższych/najdalszych po lewej i po prawej dla każdego
void prepr_furthest(void) {
    m_przed[0][0] = _arr[0];
    for (int i = 1; i < _n; ++i) {
        bool bylo = false;

        for (int j = 0; j < 3; ++j) {
            m_przed[i][j] = m_przed[i - 1][j];
            if (m_przed[i][j].col == _arr[i].col)
                bylo = true;
            if (!bylo && is_empty(m_przed[i][j])){
                m_przed[i][j] = _arr[i];
                break;
            }
        }
    }

    m_za[_n-1][0] = _arr[_n-1];
    for (int i = _n-2; i >= 0; --i) {
        bool bylo = false;

        for (int j = 0; j < 3; ++j) {
            m_za[i][j] = m_za[i + 1][j];
            if (m_za[i][j].col == _arr[i].col)
                bylo = true;
            if (!bylo && is_empty(m_za[i][j])) {
                m_za[i][j] = _arr[i];
                break;
            }
        }
    }
}
void prepr_closest(void) {
    m_przed[0][0] = _arr[0];
    for (int i = 1; i < _n; ++i) {
        bool bylo = false;

        for (int j = 0; j < 3; ++j) {
            m_przed[i][j] = m_przed[i - 1][j];
            if (m_przed[i][j].col == _arr[i].col || (!bylo && is_empty(m_przed[i][j]))) {
                // kolory takie same albo wolne miejsce
                m_przed[i][j] = _arr[i];
                bylo = true;
            }
        }
        if (!bylo) {
            m_przed[i][find_min(m_przed[i])] = _arr[i];
        }
    }

    m_za[_n-1][0] = _arr[_n-1];
    for (int i = _n-2; i >= 0; --i) {
        bool bylo = false;

        for (int j = 0; j < 3; ++j) {
            m_za[i][j] = m_za[i + 1][j];
            if (m_za[i][j].col == _arr[i].col || (!bylo && is_empty(m_za[i][j]))) {
                // kolory takie same albo wolne miejsce
                m_za[i][j] = _arr[i];
                bylo = true;
            }
        }
        if (!bylo) {
            m_za[i][find_max(m_za[i])] = _arr[i];
        }
    }
}

ll merge_ans_closest(ll wyn, ll mini, ll mid, ll maxi) {
    return max(wyn, min(mid - mini, maxi - mid));
}
ll merge_ans_furthest(ll wyn, ll mini, ll mid, ll maxi) {
    return min(wyn, max(mid - mini, maxi - mid));
}

ll solve(ll ans, ll (*merge_ans)(ll, ll, ll, ll)) {
    const ll ans_start = ans;
    for (int i = 0; i < _n; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                if (is_empty(m_przed[i][j]) || is_empty(m_za[i][k]))
                    continue;
                if (m_przed[i][j].col == m_za[i][k].col || m_przed[i][j].col == _arr[i].col || m_za[i][k].col == _arr[i].col)
                    continue;
                ans = merge_ans(ans, m_przed[i][j].pos, _arr[i].pos, m_za[i][k].pos);
            }
        }
    }
    if (ans == ans_start)
        ans = 0;
    return ans;
}

ll solve_furthest(void) {
    prepr_furthest();
    return solve(0, merge_ans_closest);
}
ll solve_closest(void) {
    prepr_closest();
    return solve(INF, merge_ans_furthest);
}

////////////////////////////
// Wczytywanie inputu i inicjalizacja m_za, m_przed na puste wartości
void init(void) {
    assert(scanf("%d", &_n));
    _arr = (M*) malloc(sizeof(M) * (size_t) _n);

    for (int i = 0; i < _n; ++i) {
        assert(scanf("%d%lld", &_arr[i].col, &_arr[i].pos));
    }

    m_przed = (M **) malloc(sizeof(M *) * (size_t) _n);
    m_za = (M **) malloc(sizeof(M*) * (size_t) _n);
    for (int i = 0; i < _n; ++i) {
        m_przed[i] = (M*) malloc(sizeof(M) * 3);
        m_za[i] = (M*) malloc(sizeof(M) * 3);
        for (int j = 0; j < 3; ++j) {
            m_przed[i][j] = M_PUSTY;
            m_za[i][j] = M_PUSTY;
        }
    }
}
// Czyści pamięć
void cleanup(void){
    free(_arr);
    for (int i = 0; i < _n; ++i) {
        free(m_przed[i]);
        free(m_za[i]);
    }
    free(m_za);
    free(m_przed);
}

int main(void) {
    init();
    if (_n < 3) {
        printf("0 0\n");
        cleanup();
        return 0;
    }
    printf("%lld %lld\n", solve_closest(), solve_furthest());
    cleanup();
    return 0;
}
