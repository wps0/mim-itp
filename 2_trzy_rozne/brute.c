#include <assert.h>
#include <stdio.h>
#include <malloc.h>
#include <limits.h>
#include <stdbool.h>

//#define _DEBUG
#define INF (LLONG_MAX/2LL)

typedef long long ll;
typedef struct Motel{
    int col; // color
    ll pos;
} M;

ll min(ll a, ll b) {
    return a > b ? b : a;
}
ll max(ll a, ll b) {
    return a < b ? b : a;
}

int _n;
M *_arr;

void brute_n3(void) {
    ll furthest = 0, closest = INF;
    for (int i = 0; i < _n; ++i) {
        for (int j = i+1; j < _n; ++j) {
            for (int k = j+1; k < _n; ++k) {
                if (_arr[i].col == _arr[j].col || _arr[i].col == _arr[k].col || _arr[j].col == _arr[k].col)
                    continue;
                furthest = max(furthest, min(_arr[j].pos - _arr[i].pos, _arr[k].pos - _arr[j].pos));
                closest = min(closest, max(_arr[j].pos - _arr[i].pos, _arr[k].pos - _arr[j].pos));
            }
        }
    }
    if (closest == INF)
        closest = 0;
    printf("%lld %lld\n", closest, furthest);
}


// closest - true jeśli szukamy najbliższego
int find(int idx, int delta, bool closest, int ign1, int ign2) {
    int pos = -1;
    while (idx >= 0 && idx < _n) {
        if (_arr[idx].col != ign1 && _arr[idx].col != ign2) {
            pos = idx;
            if (closest)
                break;
        }
        idx += delta;
    }
    return pos;
}
ll find_specific(int i, bool closest, ll ans) {
    int l1 = find(i - 1, -1, closest, _arr[i].col, -1);
    int r1 = find(i+1, 1, closest, _arr[i].col, (l1 == -1 ? -1 : _arr[l1].col));
    int r2 = find(i+1, 1, closest, _arr[i].col, -1);
    int l2 = find(i - 1, -1, closest, _arr[i].col, (r2 == -1 ? -1 : _arr[r2].col));

    if (closest) {
        if (l1 != -1 && r1 != -1)
            ans = min(ans, max(_arr[i].pos - _arr[l1].pos, _arr[r1].pos - _arr[i].pos));
        if (l2 != -1 && r2 != -1)
            ans = min(ans, max(_arr[i].pos - _arr[l2].pos, _arr[r2].pos - _arr[i].pos));
    } else {
        if (l1 != -1 && r1 != -1)
            ans = max(ans, min(_arr[i].pos - _arr[l1].pos, _arr[r1].pos - _arr[i].pos));
        if (l2 != -1 && r2 != -1)
            ans = max(ans, min(_arr[i].pos - _arr[l2].pos, _arr[r2].pos - _arr[i].pos));
    }
    return ans;
}
// O(n^2)
void brute_n2(void) {
    ll furthest = 0, closest = INF;

    for (int i = 0; i < _n; ++i) {
        closest = find_specific(i, true, closest);
        furthest = find_specific(i, false, furthest);
    }
    if (closest == INF)
        closest = 0;
    printf("%lld %lld\n", closest, furthest);
}


///////////////////
#define M_PUSTY ((M){.col = -1, .pos = -1})

bool is_empty(M motel) {
    return motel.pos < 0 || motel.col < 0;
}

// trzy polozone najblizej 0 ale m_przed i-tym o roznych kolorach
M **m_przed;
// trzy polozone najblizej konca ale m_za i-tym o roznych kolorach
M **m_za;
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

void prepr_najdalsze(void) {
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

//#ifdef _DEBUG
//    for (int i = 0; i < _n; ++i) {
//        printf("przed i=%d: (", i);
//        for (int j = 0; j < 3; ++j) {
//            printf("col=%d pos=%lld;", m_przed[i][j].col, m_przed[i][j].pos);
//        }
//        printf(")\n");
//    }
//    printf("-------\n");
//    for (int i = _n-1; i >= 0; --i) {
//        printf("za i=%d: (", i);
//        for (int j = 0; j < 3; ++j) {
//            printf("col=%d pos=%lld;", m_za[i][j].col, m_za[i][j].pos);
//        }
//        printf(")\n");
//    }
//#endif
}

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

void prepr_najblizsze(void) {
    m_przed[0][0] = _arr[0];
    for (int i = 1; i < _n; ++i) {
        bool bylo = false;

        for (int j = 0; j < 3; ++j) {
            m_przed[i][j] = m_przed[i - 1][j];
            if (m_przed[i][j].col == _arr[i].col) {
                m_przed[i][j] = _arr[i];
                bylo = true;
            } else if (!bylo && is_empty(m_przed[i][j])){
                m_przed[i][j] = _arr[i];
                bylo = true;
            }
        }
        if (!bylo) {
            // wyrzuc najmniej odlegle od 0
            m_przed[i][find_min(m_przed[i])] = _arr[i];
        }
    }

    m_za[_n-1][0] = _arr[_n-1];
    for (int i = _n-2; i >= 0; --i) {
        bool bylo = false;

        for (int j = 0; j < 3; ++j) {
            m_za[i][j] = m_za[i + 1][j];
            if (m_za[i][j].col == _arr[i].col) {
                m_za[i][j] = _arr[i];
                bylo = true;
            } else if (!bylo && is_empty(m_za[i][j])) {
                m_za[i][j] = _arr[i];
                bylo = true;
            }
        }
        if (!bylo) {
            // wyrzuc najmniej odlegle od ostatniego
            m_za[i][find_max(m_za[i])] = _arr[i];
        }
    }

#ifdef _DEBUG
    for (int i = 0; i < _n; ++i) {
        printf("przed i=%d: (", i);
        for (int j = 0; j < 3; ++j) {
            printf("col=%d pos=%lld;", m_przed[i][j].col, m_przed[i][j].pos);
        }
        printf(")\n");
    }
    printf("-------\n");
    for (int i = _n-1; i >= 0; --i) {
        printf("za i=%d: (", i);
        for (int j = 0; j < 3; ++j) {
            printf("col=%d pos=%lld;", m_za[i][j].col, m_za[i][j].pos);
        }
        printf(")\n");
    }
#endif
}


ll lacz_wynik_najdalsze(ll wyn, ll mini, ll mid, ll maxi) {
    return max(wyn, min(mid - mini, maxi - mid));
}
ll lacz_wynik_najblizsze(ll wyn, ll mini, ll mid, ll maxi) {
    return min(wyn, max(mid - mini, maxi - mid));
}

ll rozw(ll ans, ll (*merge_ans)(ll, ll, ll, ll)) {
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

ll rozw_najdalsze(void) {
    prepr_najdalsze();
    return rozw(0, lacz_wynik_najdalsze);
}
ll rozw_najblizsze(void) {
    prepr_najblizsze();
    return rozw(INF, lacz_wynik_najblizsze);
}

void cleanup(void){
    free(_arr);
    for (int i = 0; i < _n; ++i) {
        free(m_przed[i]);
    }
    free(m_przed);
}

void sol(void) {
    init();
    brute_n2();
    cleanup();
}

//void sol(void) {
//    init();
//    if (_n < 3) {
//        printf("0 0\n");
//        return;
//    }
//    printf("%lld %lld\n", rozw_najblizsze(), rozw_najdalsze());
//    cleanup();
//}

//#ifdef _DEBUG
int main(void) {
    sol();
}
//#endif
