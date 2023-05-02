//==============
// Problem:
// URL:
//==============
#include <bits/stdc++.h>

using namespace std;

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

typedef struct Point {
    double x, y;
    void read(void) {
        assert(scanf("%lf%lf", &x, &y));
    }
    string to_string(void) {
        return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
    }
    string to_string_low_precision(void) {
        char *buf = (char *) malloc(256*sizeof(char));
        assert(sprintf(buf, "(%.1lf, %.1lf)", x, y));
        string s(buf);
        free(buf);
        return s;
    }
} Point;

//  ----------
//  SOLUTION
//  ----------
const string DOTS_TEMPLATE = "%{{DOTS}}%";
const string FIG_NAME_PREFIX = "fig";
pii GRID_SIZE = {5, 5};
int n, q;
vector<string> definitions, declarations, dots;


string get_fig_name(int fig_id) {
    return FIG_NAME_PREFIX + std::to_string(fig_id);
}

string draw_rect(Point p1, Point p2) {
    return "\\drawRectangle{" + p1.to_string() + "}{" + p2.to_string() + "}";
}
string draw_circle(double r, Point s) {
    return "\\drawCircle{"+s.to_string()+"}{"+std::to_string(r)+"}";
}
string fold_figure(int fig_id, Point p1, Point p2) {
    return "\\foldFig{" + get_fig_name(fig_id) + "}{"+ to_string(p1.x) +"}{"+ to_string(p1.y) +"}{"+ to_string(p2.x) +"}{"+ to_string(p2.y) +"}";
}

void add_figure(int fig_id, string definition) {
    string fig_name = get_fig_name(fig_id);
    definitions.pb("   "+fig_name+"/.pic={\n"
                   "        "+definition+"\n"
                   "   },\n");
    dots.pb(" ");
    declarations.pb("\\begin{tikzpicture}\n"
                    "    \\drawGrid{"+ to_string(GRID_SIZE.first) +"}{"+ to_string(GRID_SIZE.second) +"}"
                    "    %{{DOTS}}%\n"
                    "    \\path pic {"+fig_name+"};\n"
                    "\\end{tikzpicture}\n");
}

void add_dot(int id, size_t pos, Point p) {
    dots[pos] += "\\node[dot] at "+p.to_string()+" {};\n";
    string label = to_string(id) + p.to_string_low_precision();
    p.y += 0.3;
    dots[pos] += "\\node at "+p.to_string()+" {"+label+"};\n";
}

void update_grid_size(Point p) {
    GRID_SIZE.first = static_cast<int>(fmax(GRID_SIZE.fi, ceil(p.x)));
    GRID_SIZE.second = static_cast<int>(fmax(GRID_SIZE.se, ceil(p.y)));
}

void read_figures() {
    int fig_id = 1;
    REP(i, n) {
        char t;
        cin >> t;
        string fig_def;
        if (t == 'P') {
            Point p1, p2;
            p1.read(), p2.read();
            update_grid_size(p1), update_grid_size(p2);
            fig_def = draw_rect(p1, p2);
        } else if (t == 'K') {
            Point s;
            s.read();
            double r;
            assert(scanf("%lf", &r));
            fig_def = draw_circle(r, s);
        } else if (t == 'Z') {
            int target;
            assert(scanf("%d", &target));
            Point p1, p2;
            p1.read(), p2.read();
            update_grid_size(p1), update_grid_size(p2);
            fig_def = fold_figure(target, p1, p2);
        } else {
            assert(false);
        }
        add_figure(fig_id++, fig_def);
    }
}
void read_dots() {
    REP(i, q) {
        size_t target;
        assert(scanf("%ld", &target));
        Point p;
        p.read();
        update_grid_size(p), update_grid_size(p);
        add_dot(i, target-1, p);
    }
}

string create_file(){
    string defs = "";
    for (string &x : definitions) {
        defs += x;
    }
    defs.erase(defs.find_last_of(','));

    int i = 0;
    string decl = "";
    for (string &x : declarations) {
        x.replace(x.find(DOTS_TEMPLATE), DOTS_TEMPLATE.length(), dots[(size_t)i]);
        decl += x;
        i++;
    }
    return "\\documentclass[tikz,border=10pt]{standalone}\n"
           "\\usepackage{origami}\n"
           "\\tikzstyle{dot}=[inner sep=0.03cm, circle, draw, fill=blue]\n"
           "\n"
           "\\tikzset{\n"
           +defs+"\n"
           "}\n"
           "\n"
           "\\begin{document}\n"
           "    "+decl+"\n"
           "\\end{document}";
}

int main() {
    assert(scanf("%d %d\n", &n, &q));
    read_figures();
    read_dots();
    assert(printf("%s", create_file().c_str()));
}