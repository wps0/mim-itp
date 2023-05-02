#include <cstdlib>
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <string>
#include "kol.h"

///////// PAMIĘĆ
// alokuje n komórek pamięci, każda rozmiaru sz
void *allocate_mem(size_t sz, size_t n = 1) {
    void *mem = calloc(n, sz);
    assert(mem);
    return mem;
}

void free_mem(void *mem) {
    assert(mem != nullptr);
    free(mem);
}

///////// IMPLEMENTACJA INTERESANTA
struct interesant {
    // wartości ujemne to anchory
    int nr;
    // pointery na sąsiadów interesanta
    interesant *l1, *l2;

    explicit interesant(int id) : nr(id), l1(nullptr), l2(nullptr) { }
};

///////// IMPLEMENTACJA LISTY
// wpina nw w miejsce old w targecie
void link(interesant *target, interesant *old, interesant *nw) {
    if (target->l1 == old)
        target->l1 = nw;
    else
        target->l2 = nw;
}

// wpina in pomiędzy sąsiadów nei1 i nei2
void link_in_between(interesant *nei1, interesant *nei2, interesant *in) {
    link(nei1, nei2, in);
    link(nei2, nei1, in);
    in->l1 = nei1;
    in->l2 = nei2;
}

// zwraca sąsiadującego interesanta ainnego niż ign1 i ign2
interesant *get_different_than(interesant *target, interesant *ign1, interesant *ign2 = nullptr) {
    if (target->l2 != ign1 && target->l2 != ign2)
        return target->l2;
    if (target->l1 != ign1 && target->l1 != ign2)
        return target->l1;
    return nullptr;
}

class InteresantList {
public:
    interesant *front;
    interesant *back;

    InteresantList() {
        remake_list();
    }

    void push_back(interesant *in) {
        if (is_empty()) {
            link_in_between(front, back, in);
        } else {
            link_in_between(back, get_not_an_anchor(back), in);
        }
    }

    interesant *pop_front() {
        assert(!is_empty());
        interesant *top = get_not_an_anchor(front);
        interesant *future_top = get_different_than(top, front);
        link(front, top, future_top);
        link(future_top, top, front);
        return top;
    }

    bool is_empty() {
        return front->l2 == back && front->l1 == back;
    }

    void reverse() {
        std::swap(front, back);
    }

    // Dorzuca listę podaną w argumencie do końca obecnej
    void merge(InteresantList *child) {
        if (child->is_empty())
            return;
        if (is_empty()) {
            free_mem(front);
            free_mem(back);
            front = child->front;
            back = child->back;
            child->remake_list();
            return;
        }
        // podpina skrajne elementy do siebie
        interesant *closer = get_not_an_anchor(back);
        interesant *further = child->get_not_an_anchor(child->front);
        link(closer, back, further);
        link(further, child->front, closer);
        // odpowiednio przepina końce obecnej listy
        link(front, back, child->back);
        link(child->back, child->front, front);

        free_mem(back);
        free_mem(child->front);
        back = child->back;

        child->remake_list();
    }
    // zwraca pointer do tego sąsiada danego interesanta, który nie jest anchorem
    interesant *get_not_an_anchor(interesant *in) {
        return get_different_than(in, front, back);
    }

private:
    // tworzy listę na nowo, ale nie zwalnia pamięci z obecnego front i back
    void remake_list() {
        front = (interesant *) allocate_mem(sizeof(interesant));
        back = (interesant *) allocate_mem(sizeof(interesant));
        *front = interesant(-1);
        *back = interesant(-2);
        front->l1 = back;
        front->l2 = back;
        back->l1 = front;
        back->l2 = front;
    }
};

///////// IMPLEMENTACJA "ŁAZIKA" I METODY NA NIM
// Łazik to struktura służąca do chodzenia po liście.
class Rover {
    const interesant *starting_pos;
    interesant *pos, *pos_prev;

public:
    Rover(interesant *st_pos, interesant *prev) : pos(st_pos) {
        starting_pos = st_pos;
        pos_prev = prev;
    }

    // Zwraca wartość, na której teraz stoi łazik
    interesant *value() {
        return pos;
    }

    // Przesuwa łazika do elementu, który nie został jeszcze odwiedzony.
    //  true - udało się przesunąć, false - lista jest pusta lub łazik obszedł ją dookoła
    bool advance() {
        interesant *nx = get_different_than(pos, pos_prev);
        if (nx == starting_pos || nx == nullptr)
            return false;
        pos_prev = pos;
        pos = nx;
        return true;
    }
};

///////// FUNKCJE POMOCNICZE
// Odłącza fragment elementów od listy, na której się teraz znajdują
void cut_elements_from_list(std::vector<interesant *> &to_be_cut) {
    interesant *cut1 = get_different_than(to_be_cut[0], to_be_cut[1]);
    interesant *cut2 = get_different_than(*to_be_cut.rbegin(), *(++to_be_cut.rbegin()));
    link(cut1, to_be_cut[0], cut2);
    link(cut2, *to_be_cut.rbegin(), cut1);
}

///////// FUNKCJE Z kol.h
int n, interesant_cnt;
InteresantList *l;

void otwarcie_urzedu(int m) {
    interesant_cnt = 0;
    n = m;
    l = (InteresantList *) allocate_mem(sizeof(InteresantList), (size_t) n);
    for (int i = 0; i < n; ++i) {
        l[i] = InteresantList();
    }
}

interesant *nowy_interesant(int k) {
    interesant *i = (interesant *) allocate_mem(sizeof(interesant));
    i->nr = interesant_cnt++;
    l[k].push_back(i);
    return i;
}

int numerek(interesant *i) {
    return i->nr;
}

interesant *obsluz(int k) {
    if (l[k].is_empty())
        return NULL;
    return l[k].pop_front();
}

void zmiana_okienka(interesant *i, int k) {
    link(i->l2, i, i->l1);
    link(i->l1, i, i->l2);
    l[k].push_back(i);
}

void zamkniecie_okienka(int k1, int k2) {
    l[k2].merge(&l[k1]);
}

std::vector<interesant *> fast_track(interesant *i1, interesant *i2) {
    if (i1 == i2) {
        link(i1->l2, i1, i1->l1);
        link(i1->l1, i1, i1->l2);
        return std::vector<interesant *>{i1};
    }
    // Idzie w dwie strony listy zaczynając od i1 dopóki nie napotka i2 zapisując ścieżkę w trakcie.
    std::vector<interesant *> path_left, path_right;
    Rover left(i1, get_different_than(i1, i1->l1));
    Rover right(i1, get_different_than(i1, i1->l2));

    while (left.value() != i2 && right.value() != i2) {
        // jeśli numerki są ujemne to łazik doszedł do początku/końca listy
        if (left.value()->nr >= 0) {
            path_left.push_back(left.value());
            left.advance();
        }
        if (right.value()->nr >= 0){
            path_right.push_back(right.value());
            right.advance();
        }
    }
    // na ścieżkę nie została wrzucona jeszcze ostatnia wartość
    path_right.push_back(right.value());
    path_left.push_back(left.value());

    if (left.value() == i2) {
        cut_elements_from_list(path_left);
        return path_left;
    }
    cut_elements_from_list(path_right);
    return path_right;
}

void naczelnik(int k) {
    l[k].reverse();
}

std::vector<interesant *> zamkniecie_urzedu() {
    std::vector<interesant *> remaining;
    for (int i = 0; i < n; ++i) {
        Rover r(l[i].front, l[i].back);
        while (r.advance() && r.value()->nr >= 0) {
            remaining.push_back(r.value());
        }
        free_mem(l[i].front);
        free_mem(l[i].back);
    }
    free_mem(l);
    return remaining;
}
