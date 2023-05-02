#include <stdlib.h>
#include <math.h>
//#include <assert.h>
//#include <stdio.h>
#include <unistd.h>
#include "ary.h"

///========== DEFINICJE
//////////////////////////////////////////
#define ZERO_EPSILON 1e-10

#define PRZEDZIAL_PUSTY ((Przedzial){ \
    .dol = NAN, .gora = NAN          \
})
#define PRZEDZIAL_INF ((Przedzial){ \
    .dol = -HUGE_VAL, .gora = HUGE_VAL \
})
#define SUMA_PRZEDZIALOW_PUSTA ((SumaPrzedzialow){ \
    .p_mniejszy = PRZEDZIAL_PUSTY,                \
    .p_wiekszy = PRZEDZIAL_PUSTY                  \
})
#define SUMA_PRZEDZIALOW_INF ((SumaPrzedzialow){ \
    .p_mniejszy = PRZEDZIAL_INF,                \
    .p_wiekszy = PRZEDZIAL_PUSTY                \
})
#define WARTOSC_PUSTA stworz_wartosc(SUMA_PRZEDZIALOW_PUSTA)

///========== STRUKTURY
//////////////////////////////////////////

typedef struct Przedzialy {
    size_t ilosc;
    Przedzial *p;
} Przedzialy;

///========== FUNKCJE NA STRUKTURACH
//////////////////////////////////////////

Przedzialy stworz_przedzialy(size_t ile) {
    Przedzialy p = (Przedzialy){
            .ilosc = ile,
            .p = (Przedzial*)malloc(sizeof(Przedzial)*ile)
    };
    for (size_t i = 0; i < ile; i++) {
        p.p[i] = PRZEDZIAL_PUSTY;
    }
    return p;
}
inline void usun_przedzialy(Przedzialy p) {
    free(p.p);
}

inline wartosc stworz_wartosc(SumaPrzedzialow sp) {
    return (wartosc){
        .sp = sp
    };
}

inline Przedzial stworz_przedzial(double dol, double gora) {
    return (Przedzial){
        .dol = dol,
        .gora = gora
    };
}

///========== FUNKCJE POMOCNICZE MAŁE
//////////////////////////////////////////
inline double min(double a, double b) {
    return a < b ? a : b;
}
inline double max(double a, double b) {
    return a < b ? b : a;
}
// Ignoruje NANy
inline double minn(double a, double b) {
    if (isnan(a))
        return b;
    if (isnan(b))
        return a;
    return min(a, b);
}
inline double maxn(double a, double b) {
    if (isnan(a))
        return b;
    if (isnan(b))
        return a;
    return max(a, b);
}

inline void swapd(double *a, double *b) {
    double c = *a;
    *a = *b;
    *b = c;
}
inline void swapp(Przedzial *a, Przedzial *b) {
    Przedzial c = *a;
    *a = *b;
    *b = c;
}

inline bool czy_rowne(double a, double b) {
    if (isinf(a) == isinf(b) && isinf(a) != 0)
        return true;
    return fabs(a-b) < ZERO_EPSILON;
}

// jak to sie zachowuje dla nanów?
inline bool czy_zerowy(Przedzial p) {
    return czy_rowne(p.dol, 0) && czy_rowne(p.gora, 0);
}
inline bool czy_pusty(Przedzial p) {
    return isnan(p.dol) && isnan(p.gora);
}
inline bool czy_sp_pusta(SumaPrzedzialow sp) {
    return czy_pusty(sp.p_mniejszy) && czy_pusty(sp.p_wiekszy);
}
inline bool czy_nieskonczony(Przedzial p) {
    return -isinf(p.dol) && isinf(p.gora);
}

inline bool czy_lezy_pomiedzy(double x, double dol, double gora) {
    if (isnan(dol) || isnan(gora) || isnan(x))
        return false;
    if (czy_rowne(x, dol) || czy_rowne(x, gora))
        return true;
    return dol < x && x < gora;
}
// Zwraca true jeśli przedziały na siebie nachodzą.
inline bool czy_sie_przecinaja(Przedzial p1, Przedzial p2) {
    return czy_lezy_pomiedzy(p1.dol, p2.dol, p2.gora) || czy_lezy_pomiedzy(p1.gora, p2.dol, p2.gora)
        || czy_lezy_pomiedzy(p2.dol, p1.dol, p1.gora) || czy_lezy_pomiedzy(p2.gora, p1.dol, p1.gora);
}
inline bool czy_w_przedziale(double x, Przedzial p) {
    return czy_sie_przecinaja((Przedzial){.dol = x, .gora = x}, p);
}

///========== FUNKCJE POMOCNICZE DUŻE
//////////////////////////////////////////
inline Przedzial polacz_przedzialy(Przedzial p1, Przedzial p2) {
    return (Przedzial){
            .dol = min(p1.dol, p2.dol),
            .gora = max(p1.gora, p2.gora)
    };
}
size_t rozbij_przedzial(Przedzialy *rozbite, size_t offset, Przedzial p) {
    size_t idx = offset;
    if (p.dol < 0 && p.gora > 0) {
        rozbite->p[idx++] = stworz_przedzial(p.dol, -0.0);
        rozbite->p[idx++] = stworz_przedzial(0.0, p.gora);
    } else if (!czy_rowne(p.dol, 0) && czy_rowne(p.gora, 0)){
        p.gora = -0.0;
        rozbite->p[idx++] = p;
    } else {
        rozbite->p[idx++] = p;
    }
    return idx;
}
Przedzialy sortuj_przedzialy(Przedzialy p) {
    for (size_t i = 0; i < p.ilosc; ++i) {
        for (size_t j = 1; j < p.ilosc-i; ++j) {
            if (p.p[j-1].dol > p.p[j].dol) {
                swapp(&p.p[j-1], &p.p[j]);
            }
        }
    }
    return p;
}

// Rozbija wartość na przedziały takie, żeby żaden z nich nie zawierał liczb o przeciwnych znakach.
//  Np. [-inf, 0] zamieni się na [-inf, -0]
Przedzialy rozbij(wartosc w) {
    Przedzialy rozbite = stworz_przedzialy(3);

    size_t idx = rozbij_przedzial(&rozbite, 0, w.sp.p_mniejszy);
    if (!czy_pusty(w.sp.p_wiekszy))
        idx += rozbij_przedzial(&rozbite, idx, w.sp.p_wiekszy);
//    assert(idx <= 3);
    return rozbite;
}
// Robi sumę teoriomnogościową przedziałów i opakowuje je w wartość.
wartosc polacz(Przedzialy p) {
    if (p.ilosc == 0)
        return stworz_wartosc(SUMA_PRZEDZIALOW_PUSTA);
    p = sortuj_przedzialy(p);

    SumaPrzedzialow suma = SUMA_PRZEDZIALOW_PUSTA;
    for (size_t i = 0; i < p.ilosc; i++) {
        if (czy_pusty(p.p[i]))
            continue;
        if (czy_pusty(suma.p_mniejszy))
            suma.p_mniejszy = p.p[i];
        else if (czy_sie_przecinaja(suma.p_mniejszy, p.p[i]))
            suma.p_mniejszy = polacz_przedzialy(suma.p_mniejszy, p.p[i]);
        else if (czy_pusty(suma.p_wiekszy))
            suma.p_wiekszy = p.p[i];
        else if (czy_sie_przecinaja(p.p[i], suma.p_wiekszy))
            suma.p_wiekszy = polacz_przedzialy(suma.p_wiekszy, p.p[i]);
//        else {
            // przedziały rozdzieliły się na trzy rozłączne - to nie powinno nigdy nastąpić
//            assert(false);
//        }
    }
    return stworz_wartosc(suma);
}

wartosc wykonaj_dzialanie(wartosc a, wartosc b, Przedzial (*operacja)(Przedzial, Przedzial)) {
    // Operacje wykonywane na zbiorach pustym i niepustym mają dawać zbiór pusty
    if (czy_pusty(a.sp.p_mniejszy) || czy_pusty(b.sp.p_mniejszy))
        return WARTOSC_PUSTA;
    Przedzialy op_lhs = stworz_przedzialy(2);
    Przedzialy op_rhs = rozbij(b);
    Przedzialy wyniki = stworz_przedzialy(10);
    size_t idx_wyniki = 0;

    op_lhs.p[0] = a.sp.p_mniejszy;
    op_lhs.p[1] = a.sp.p_wiekszy;

    for (size_t i = 0; i < op_lhs.ilosc; i++) {
        if (czy_pusty(op_lhs.p[i]))
            continue;
        for (size_t j = 0; j < op_rhs.ilosc; j++) {
            if (czy_pusty(op_rhs.p[j]))
                continue;
            Przedzial l = op_lhs.p[i], p = op_rhs.p[j];
            wyniki.p[idx_wyniki++] = operacja(l, p);
        }
    }
//    assert(idx_wyniki <= 10);

    wartosc wynik = polacz(wyniki);
    usun_przedzialy(op_lhs);
    usun_przedzialy(op_rhs);
    usun_przedzialy(wyniki);
    return wynik;
}

// działania - zastępują podstawowe operacje takie jak +, *, /, ale mają wyifowane corner case'y
double dzialanie_mnozenie(double a, double b) {
    if (czy_rowne(a, 0) || czy_rowne(b, 0))
        return 0;
    return a*b;
}

Przedzial operacja_dzielenie(Przedzial lhs, Przedzial rhs) {
    return stworz_przedzial(
            minn(minn(lhs.dol / rhs.gora, lhs.dol / rhs.dol), minn(lhs.gora / rhs.dol, lhs.gora / rhs.gora)),
            maxn(maxn(lhs.dol / rhs.gora, lhs.dol / rhs.dol), maxn(lhs.gora / rhs.dol, lhs.gora / rhs.gora))
    );
}
Przedzial operacja_dodawanie(Przedzial lhs, Przedzial rhs) {
    return stworz_przedzial(
            minn(minn(lhs.dol + rhs.gora, lhs.dol + rhs.dol), minn(lhs.gora + rhs.dol, lhs.gora + rhs.gora)),
            maxn(maxn(lhs.dol + rhs.gora, lhs.dol + rhs.dol), maxn(lhs.gora + rhs.dol, lhs.gora + rhs.gora))
    );
}
Przedzial operacja_mnozenie(Przedzial lhs, Przedzial rhs) {
    return stworz_przedzial(
            minn(minn(dzialanie_mnozenie(lhs.dol, rhs.gora), dzialanie_mnozenie(lhs.dol, rhs.dol)),
                 minn(dzialanie_mnozenie(lhs.gora, rhs.dol), dzialanie_mnozenie(lhs.gora, rhs.gora))),
            maxn(maxn(dzialanie_mnozenie(lhs.dol, rhs.gora), dzialanie_mnozenie(lhs.dol, rhs.dol)),
                 maxn(dzialanie_mnozenie(lhs.gora, rhs.dol), dzialanie_mnozenie(lhs.gora, rhs.gora)))
    );
}

Przedzial odwrotnosc(Przedzial p) {
    if (czy_pusty(p))
        return p;
    swapd(&p.dol, &p.gora);
    p.dol *= -1;
    p.gora *= -1;
    return p;
}
wartosc odwrotnosc_w(wartosc w) {
    w.sp.p_mniejszy = odwrotnosc(w.sp.p_mniejszy);
    if (!czy_pusty(w.sp.p_wiekszy)){
        w.sp.p_wiekszy = odwrotnosc(w.sp.p_wiekszy);
        swapp(&w.sp.p_mniejszy, &w.sp.p_wiekszy);
    }
    return w;
}

///========== IMPLEMENTACJA FUNKCJI Z ary.h
//////////////////////////////////////////

wartosc wartosc_od_do(double x, double y) {
    // do obsługi przypadków, w których np. x = 1e-11, a to jest równe 0
    if (czy_rowne(x, 0))
        x = 0;
    if (czy_rowne(y, 0))
        y = 0;
    return stworz_wartosc((SumaPrzedzialow){
        .p_mniejszy = (Przedzial){.dol = x, .gora = y},
        .p_wiekszy = PRZEDZIAL_PUSTY
    });
}
wartosc wartosc_dokladnosc(double x, double p) {
    p /= 100;
    return wartosc_od_do(min(x-x*p, x+x*p), max(x-x*p, x+x*p));
}
wartosc wartosc_dokladna(double x) {
    return wartosc_od_do(x, x);
}

bool in_wartosc(wartosc w, double x) {
    return czy_w_przedziale(x, w.sp.p_mniejszy) || czy_w_przedziale(x, w.sp.p_wiekszy);
}
double min_wartosc(wartosc w) {
    if (czy_sp_pusta(w.sp))
        return NAN;
    // wartosc najmniejsza musi byc w mniejszym przedziale.dol
    return w.sp.p_mniejszy.dol;
}
double max_wartosc(wartosc w) {
    if (czy_sp_pusta(w.sp))
        return NAN;
//    assert(!czy_pusty(w.sp.p_mniejszy));

    double mx = maxn(w.sp.p_mniejszy.gora, w.sp.p_wiekszy.gora);
//    assert(!isnan(mx)); // todo: pozbyc sie tego
    return mx;
}
double sr_wartosc(wartosc w) {
    if (czy_sp_pusta(w.sp) || czy_nieskonczony(w.sp.p_mniejszy))
        return NAN;
    return min_wartosc(w)/2 + max_wartosc(w)/2;
}

wartosc plus(wartosc a, wartosc b) {
    return wykonaj_dzialanie(a, b, operacja_dodawanie);
}

wartosc minus(wartosc a, wartosc b) {
    return wykonaj_dzialanie(a, odwrotnosc_w(b), operacja_dodawanie);
}

wartosc razy(wartosc a, wartosc b) {
    return wykonaj_dzialanie(a, b, operacja_mnozenie);
}

wartosc podzielic(wartosc a, wartosc b) {
    // dzielenie przez 0 ma zwrócić wartość pustą
    if (czy_zerowy(b.sp.p_mniejszy))
        return WARTOSC_PUSTA;
    return wykonaj_dzialanie(a, b, operacja_dzielenie);
}
