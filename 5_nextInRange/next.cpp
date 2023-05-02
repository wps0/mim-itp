#include <memory>
#include <vector>
#include <map>
#include <algorithm>
#include <climits>
#include <cstdio>
#include <set>
#include "next.h"

using namespace std;

typedef pair<int, int> pii;
const int INF = INT_MAX;
/////////
// ile wartosci na wejsciu; rozmiar drzewa (potega 2-ki)
size_t n, R;
// pary <wartość, przeskalowana wartość>
set<pii> normalised;

// największa wartość mniejsza niż val albo -1 gdy takiej nie ma
int get_normalised_lt(int val) {
    auto lb = normalised.lower_bound({val, -1});
    if (lb == normalised.begin())
        return -1;
    return (--lb)->second;
}
// najmniejsza wartość większa niż val
int get_normalised_gt(int val) {
    auto lb = normalised.lower_bound({val, INF});
    if (lb == normalised.end())
        return -1;
    return lb->second;
}

///////// WĘZEŁ DRZEWA PRZEDZIAŁOWEGO
struct Node {
    int val;
    shared_ptr<Node> l, r;

    Node() {
        val = INF;
        l = shared_ptr<Node>();
        r = shared_ptr<Node>();
    }
};

///////// METODY POMOCNICZE
// zwraca najmniejszą potęgę 2-ki większą od k
int exp2(int k) {
    int ans = 1;
    while (ans < k)
        ans *= 2;
    return ans;
}

// skalowanie
vector<int> rescale(const vector<int> &vals) {
    map<int, int> scaled;
    for (int val : vals) {
        scaled[val] = 0;
    }

    int idx_next = 0;
    // mapa jest posortowana - dlatego ten for wyjmuje elementy od najmniejszego klucza,
    //  czyli od najmniejszej wartosci
    for (pair<const int, int> &x : scaled) {
        x.second = idx_next++;
        normalised.insert({x.first, x.second});
    }

    vector<int> new_vals;
    for (int x : vals) {
        new_vals.push_back(scaled[x]);
    }
    return new_vals;
}

// update drzewa przedziałowego. Zwraca nowozaalokowany wierzchołek dziecka
// p, k muszą być potęgami 2-ki
// idx - ktory indeks w drzewie zmienic
// val - na co zmienic
shared_ptr<Node> update(shared_ptr<Node> v, const int idx, const int val, int p, int k) {
    if (!v){
        v = make_shared<Node>();
    }
    if (p == idx && k == idx) {
        Node *w = new Node();
        w->val = val;
        return shared_ptr<Node>(w);
    }

    shared_ptr<Node> new_v = make_shared<Node>();
    if (idx <= (p + k) / 2){
        new_v->l = update(v->l, idx, val, p, (p + k) / 2);
        new_v->r = v->r;
    } else {
        new_v->r = update(v->r, idx, val, (p + k) / 2 + 1, k);
        new_v->l = v->l;
    }
    new_v->val = min(v->val, val);
    return new_v;
}

// p, k - poczatek/koniec przedzialu, ktory jest pod v
// l, r - poczatek/koniec przedzialu, o ktory jest query
int query(const shared_ptr<Node> &v, const int min_idx, const int l, const int r, const int p, const int k) {
    if (!v || r < p || k < l || v->val < min_idx)
        return INF;
    if (l <= p && k <= r)
        return v->val;
    return min(query(v->l, min_idx, l, r, p, (p+k)/2),
               query(v->r, min_idx, l, r, (p+k)/2 + 1, k));
}

///////// METODY Z NEXT.H
// korzenie drzewa przedziałowego w i-tej chwili (zawiera przedzial <i, INF>
vector<shared_ptr<Node>> roots;

void init(const vector<int> &x) {
    n = x.size();
    R = (size_t)exp2((int)n);
    vector<int> x_scaled = rescale(x);

    // mniejszy narzut:
    roots.resize(n+1, nullptr);
    roots[n] = make_shared<Node>();

    for (int i = (int)n-1; i >= 0; i--) {
        roots[(size_t)i] = update(roots[(size_t)i+1], x_scaled[(size_t)i], i, 0, (int)R-1);
    }
}

int nextInRange(int i, int a, int b) {
    a = max(0, get_normalised_lt(a)+1);
    b = get_normalised_gt(b)-1;
    if (b == -2)
        b = (int)n-1;

    int ans = query(roots[(size_t)i], i, a, b, 0, (int)R-1);
    if (ans == INF)
        return -1;
    return ans;
}

void done() {
    normalised.clear();
    roots.clear();
}
