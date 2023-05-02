/* kompilacja: gcc @opcje1 przyklad.c ary.c -o przyklad.e */
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <assert.h>

#include "ary.h"

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
#define ZERO_EPSILON 1e-12

inline bool czy_rowne(double x, double y) {
    return fabs(x-y) < ZERO_EPSILON;
}

//char *wartosc_jako_string(SumaPrzedzialow sp) {
//    const int buf_roz = 256;
//    char *buf = malloc(sizeof(char)*256);
//    snprintf(buf, buf_roz, "<%.10lf, %.10lf> u <%.10lf, %.10lf>", sp.p_mniejszy.dol,
//             sp.p_mniejszy.gora, sp.p_wiekszy.dol, sp.p_wiekszy.gora);
//    return buf;
//}
#define UNUSED(x) (void)(x)
//void dbg(wartosc w, char *note) {
//    UNUSED(w), UNUSED(note);
//    printf("=== DEBUG %s: %s\n", note, suma_przedzialow_to_string(w.sp));
//}

void testy_inne(void) {
    wartosc a = wartosc_dokladna(7.9999999999999);

    assert(in_wartosc(a, 8));
    assert(!in_wartosc(a, 8.000000001));
    assert(!in_wartosc(a, 7.9999999));

    wartosc b = wartosc_od_do(NAN, NAN);
    assert(isnan(min_wartosc(b)));
    assert(isnan(max_wartosc(b)));
    assert(isnan(sr_wartosc(b)));
    assert(!in_wartosc(b, 0));
    assert(!in_wartosc(b, -1e10));
    assert(!in_wartosc(b, 1e10));

    wartosc c = wartosc_dokladna(0);
    wartosc d = razy(b, c); // [nan, nan]
    assert(isnan(min_wartosc(d)));
    assert(isnan(max_wartosc(d)));
    assert(isnan(sr_wartosc(d)));
    assert(!in_wartosc(d, 0));
    assert(!in_wartosc(d, -1e10));
    assert(!in_wartosc(d, 1e10));

    wartosc e = wartosc_od_do(-HUGE_VAL, HUGE_VAL);
    wartosc f = razy(e, c); // [0; 0]
    assert(czy_rowne(0, min_wartosc(f)));
    assert(czy_rowne(0, max_wartosc(f)));
    assert(czy_rowne(0, sr_wartosc(f)));
    assert(in_wartosc(f, 0));
    assert(!in_wartosc(f, -1e10));
    assert(!in_wartosc(f, 1e10));

    wartosc g = razy(c, d); // [nan, nan]
    assert(isnan(min_wartosc(g)));
    assert(isnan(max_wartosc(g)));
    assert(isnan(sr_wartosc(g)));
    assert(!in_wartosc(g, 0));
    assert(!in_wartosc(g, -1e10));
    assert(!in_wartosc(g, 1e10));

//    wartosc h = razy(f, e); // [nan, nan]
//    assert(isnan(min_wartosc(h)));
//    assert(isnan(max_wartosc(h)));
//    assert(isnan(sr_wartosc(h)));
//    assert(!in_wartosc(h, 0));
//    assert(!in_wartosc(h, -1e10));
//    assert(!in_wartosc(h, 1e10));

    wartosc i = podzielic(wartosc_dokladna(1), wartosc_od_do(-1, 1));  //[-inf, -1] u [1, inf]
    wartosc j = razy(i, f);
//    dbg(j, "j");
    assert(czy_rowne(0, min_wartosc(j)));
    assert(czy_rowne(0, max_wartosc(j)));
    assert(czy_rowne(0, sr_wartosc(j)));
    assert(in_wartosc(j, 0));
    assert(!in_wartosc(j, -1e10));
    assert(!in_wartosc(j, 1e10));


    printf("Testy inne: OK\n");
}

void testy_suma(void) {
    wartosc a = wartosc_dokladna(0);
    wartosc b = wartosc_od_do(-0.000003, 0.5);
    wartosc c = plus(a, b); // [-0.000003, 0.5]

    assert(in_wartosc(c, -0.000003));
    assert(in_wartosc(c, 0.5));
    assert(czy_rowne(min_wartosc(c), -0.000003));
    assert(czy_rowne(max_wartosc(c), 0.5));
    assert(!in_wartosc(c, -0.00000301));
    assert(!in_wartosc(c, 0.500000001));
    printf("Testy suma: OK\n");
}
void testy_mnozenie(void) {
    wartosc a = wartosc_od_do(-1, -0.5);
    wartosc b = wartosc_od_do(-3, -1.5);
    wartosc c = razy(a, b); //[0.75, 3]
//    dbg(c, "c");

    assert(in_wartosc(c, 0.75));
    assert(in_wartosc(c, 3));
    assert(czy_rowne(min_wartosc(c), 0.75));
    assert(czy_rowne(max_wartosc(c), 3));
    assert(!in_wartosc(c, 0.74999999));
    assert(!in_wartosc(c, 3.00000001));

    wartosc d = wartosc_dokladna(NAN);
    wartosc e = razy(d, c);

    assert(isnan(min_wartosc(e)));
    assert(isnan(max_wartosc(e)));

    printf("Testy mnożenie: OK\n");
}

void testy_dzielenie(void) {
    // ile to <1, inf> / <0, 1>
    wartosc a = wartosc_od_do(-HUGE_VAL, -1);
    wartosc b = wartosc_od_do(-HUGE_VAL, -10);
    wartosc c = podzielic(a, b); // [0, inf]?

    assert(in_wartosc(c, 0.0));
    assert(!in_wartosc(c, -0.0001));
    assert(in_wartosc(c, 1000000.0));

    wartosc d = wartosc_od_do(1, 2);
    wartosc e = podzielic(d, d); // [0.5, 2]

    assert(czy_rowne(min_wartosc(e), 0.5));
    assert(czy_rowne(max_wartosc(e), 2));
    assert(!in_wartosc(e, 2.001));
    assert(!in_wartosc(e, 0.499));
    assert(in_wartosc(e, 1.23));
    assert(!in_wartosc(e, -100));
    assert(!in_wartosc(e, 100));

    wartosc f = wartosc_od_do(-1, 5);
    wartosc g = wartosc_od_do(5, HUGE_VAL);
    wartosc h = podzielic(f, g); // [-0.2, 1]

    assert(czy_rowne(max_wartosc(h), 1));
    assert(czy_rowne(min_wartosc(h), -0.2));
    assert(!in_wartosc(h, 1.00005));
    assert(!in_wartosc(h, -0.20005));
    assert(in_wartosc(h, 0));
    assert(!in_wartosc(h, -100));
    assert(!in_wartosc(h, 100));

    wartosc i = wartosc_od_do(-1, 2);
    wartosc j = podzielic(d, i); // [-inf, -1] u [0.5, inf]
//    dbg(d, "d");
//    dbg(i, "i");
//    dbg(j, "j = d/i");
    assert(-isinf(min_wartosc(j)));
    assert(isinf(max_wartosc(j)));
    assert(in_wartosc(j, -1));
    assert(in_wartosc(j, 0.5));
    assert(!in_wartosc(j, -0.9999));
    assert(!in_wartosc(j, 0.49999));

    wartosc k = wartosc_dokladna(NAN);
    wartosc l = podzielic(j, k);

    assert(isnan(min_wartosc(l)));
    assert(isnan(max_wartosc(l)));

    wartosc m = wartosc_od_do(-HUGE_VAL, HUGE_VAL);
    wartosc n = wartosc_od_do(1, 1);

    // todo: inf/1 ile to?
    wartosc o = podzielic(m, n); // [-inf, inf]
    assert(-isinf(min_wartosc(o)));
    assert(isinf(max_wartosc(o)));
    assert(isnan(sr_wartosc(o)));
    assert(in_wartosc(o, 10000));
    assert(in_wartosc(o, -10000));
    assert(in_wartosc(o, 0));
    assert(in_wartosc(o, 0.5));
    assert(in_wartosc(o, -0.5));

    wartosc p = podzielic(m, j);
    assert(isinf(min_wartosc(p)));
    assert(isinf(max_wartosc(p)));
    assert(isnan(sr_wartosc(p)));
    assert(in_wartosc(p, 10000));
    assert(in_wartosc(p, -10000));
    assert(in_wartosc(p, 0));
    assert(in_wartosc(p, 0.5));
    assert(in_wartosc(p, -0.5));

    wartosc q = podzielic(n, wartosc_od_do(1e-11, 3));
    assert(isinf(max_wartosc(q)));
    assert(czy_rowne(min_wartosc(q), 1/3.0));
    assert(isinf(sr_wartosc(q)));
    assert(in_wartosc(q, 1e13));
    assert(!in_wartosc(q, -10000));
    assert(!in_wartosc(q, 0));
    assert(!in_wartosc(q, 0.33333332));
    assert(in_wartosc(q, 0.33333334));

    printf("Testy dzielenie: OK\n");
}

void testy_rozne(void) {
//    wartosc a = razy (wartosc_od_do (0.0240251, 1.67791), podzielic (plus (wartosc_od_do (-1491.86, -109.805), wartosc_od_do (-6915.42, 0.779628)), razy (wartosc_od_do (-5012.84, 0.334874), wartosc_od_do (-0.613748, 2389.39))));
    // w przyblizeniu [-inf, -0.000851] u  [0.000000, inf]
//    dbg(a, "a");
//    wartosc b = podzielic (wartosc_od_do (0.628105, 3525.35), wartosc_od_do (-15223.8, 1.2292));
//    dbg(b, "b");

    printf("Testy rozne: OK\n");
}

int main(void) {
    testy_mnozenie();
    testy_suma();
    testy_inne();
    testy_dzielenie();
    testy_rozne();
    return 0;
}
