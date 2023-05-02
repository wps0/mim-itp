#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>

typedef struct Point {
    double x, y;
} Point;

typedef struct Shape Shape;

typedef struct Circle {
    Point s;
    double r;
} Circle;
typedef struct Rectangle {
    // lewy dolny i prawy górny punkt
    Point ld, pg;
} Rectangle;
typedef struct Reflection {
    // dwa punkty, przez które przechodzi prosta
    Point p1, p2;
    // która figura odbijana
    Shape *target;
} Reflection;

typedef enum ShapeType {
    CIRCLE, RECTANGLE, REFLECTION
} ShapeType;
typedef struct Shape {
    ShapeType type;
    union {
        Circle c;
        Rectangle r;
        Reflection refl;
    } rep;
} Shape;

typedef struct Query {
    Point p;
    // której figury dotyczy zapytanie
    Shape *target;
} Query;

///////
#define EPSILON 1e-10

void *safe_malloc(size_t sz) {
    void *mem = malloc(sz);
    assert(mem);
    return mem;
}

///////
int n, q;
// figury
Shape *shapes;
// zapytania o ilość punktów
Query *querries;


/////// WCZYTYWANIE DANYCH I INICJALIZACJA
Point read_point(void) {
    double x, y;
    assert(scanf("%lf%lf", &x, &y));
    return (Point){.x = x, .y = y};
}

void read_shapes(void) {
    for (int i = 0; i < n; ++i) {
        char type;
        assert(scanf(" %c", &type));
        Shape s;
        if (type == 'P') {
            s.type = RECTANGLE;
            s.rep.r.ld = read_point();
            s.rep.r.pg = read_point();
        } else if (type == 'K') {
            s.type = CIRCLE;
            s.rep.c.s = read_point();
            assert(scanf("%lf", &s.rep.c.r));
        } else if (type == 'Z') {
            int target;
            assert(scanf("%d", &target));
            s.type = REFLECTION;
            s.rep.refl.p1 = read_point();
            s.rep.refl.p2 = read_point();
            s.rep.refl.target = &shapes[target - 1];
        } else {
            assert(false);
        }
        shapes[i] = s;
    }
}

void read_queries(void) {
    for (int i = 0; i < q; ++i) {
        int target;
        assert(scanf("%d", &target));
        querries[i] = (Query) {
                .target = &shapes[target - 1],
                .p = read_point()
        };
    }
}


void init(void) {
    assert(scanf("%d%d", &n, &q));
    shapes = (Shape*) safe_malloc(sizeof(Shape) * (size_t) n);
    querries = (Query*) safe_malloc(sizeof(Query) * (size_t) q);

    read_shapes();
    read_queries();
}

void cleanup(void) {
    free(shapes);
    free(querries);
}

/////// ROZW
double dist_square(Point p1, Point p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return dx*dx + dy*dy;
}

bool equals(double a, double b) {
    return fabs(a - b) <= EPSILON;
}
bool is_in(Shape const *s, Point p) {
    if (s->type == RECTANGLE) {
        Point ld = s->rep.r.ld, pg = s->rep.r.pg;
        return ld.x <= p.x && p.x <= pg.x && ld.y <= p.y && p.y <= pg.y;
    }
    if (s->type == CIRCLE) {
        double r = s->rep.c.r;
        return dist_square(s->rep.c.s, p) <= r*r + EPSILON*EPSILON;
    }
//    assert(false);
    return false;
}
// http://informatyka.wroc.pl/node/455?page=0,2
double iloczyn(Point p1, Point p2, Point p3) {
    return (p2.x - p1.x)*(p3.y - p1.y) - (p3.x - p1.x)*(p2.y - p1.y);
}

// odbicie punktu względem prostej
Point reflect(Point p, Reflection t) {
    // prosta przechodzaca przez R.p1 i R.p2
    double y_diff = t.p2.y - t.p1.y, x_diff = t.p2.x - t.p1.x;
    // Prosta w postaci Ay + Bx + C = 0
    double A = x_diff;
    double B = -y_diff;
    double C = t.p1.x*y_diff - t.p1.y*x_diff;
    double AB_square = A*A + B*B;
    // https://math.stackexchange.com/a/1743581
    return (Point){.x = (p.x*(A*A-B*B) - 2*B*(A*p.y + C)) / AB_square, .y = (p.y*(B*B - A*A) - 2*A*(B*p.x + C)) / AB_square};
}

// stan(k, P) = stan(k-1, P) + stan(k-1, P')
//  P' - odbite P względem prostej
//  k - ile zgięć zostało już zaaplikowanych
int solve(Shape *s, Point p) {
    if (s->type != REFLECTION)
        return is_in(s, p);
    // punkt leży na prostej, względem której odbijam
    if (equals(iloczyn(s->rep.refl.p1, s->rep.refl.p2, p), 0))
        return solve(s->rep.refl.target, p); // = stan(k-1, P)
    // punkt leży po tej stronie prostej, po której nie ma juz figury
    if (iloczyn(s->rep.refl.p1, s->rep.refl.p2, p) < 0)
        return 0;
    // ilość kartek w punkcie p po złożeniu to ilość kartek w p przed złożeniem + ilość kartek w punkcie p', czyli
    //  p odbitym  względem prostej
    int stan1 = solve(s->rep.refl.target, p);
    int stan2 = solve(s->rep.refl.target, reflect(p, s->rep.refl));
    return stan1 + stan2; // = stan(k-1, P) + stan(k-1, P')
}

int main(void) {
    init();
    for (int i = 0; i < q; ++i) {
        printf("%d\n", solve(querries[i].target, querries[i].p));
    }
    cleanup();
}
