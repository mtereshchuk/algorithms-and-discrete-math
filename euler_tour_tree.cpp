#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

int n, m;

struct vertex {
    int number, cnt, priority;
    vertex *l, *r, *p;
    vertex(int num) {
        number = num;
        cnt = 1;
        priority = rand();
        l = nullptr;
        r = nullptr;
        p = nullptr;
    }
};

unordered_map <int, vertex *> vertex_ptr;
unordered_map <int, unordered_map <int, vertex *>> edge_ptr;

int get_cnt(vertex *v) {
    if (v != nullptr) {
        return v->cnt;
    }
    return 0;
}

void upd(vertex *v) {
    if (v != nullptr) {
        v->cnt = get_cnt(v->l) + get_cnt(v->r) + 1;
    }
}

pair <vertex *, vertex *> split(vertex *v, int key) {
    if (v == nullptr) {
        return make_pair((vertex *) nullptr, (vertex *) nullptr);
    }
    int cur_key;
    if (v->l != nullptr) {
        cur_key = v->l->cnt;
    } else {
        cur_key = 0;
    }
    if (cur_key >= key) {
        pair <vertex *, vertex *> p = split(v->l, key);
        v->l = p.second;
        upd(v);
        if (p.second != nullptr) {
            p.second->p = v;
        }
        v->p = nullptr;
        if (p.first != nullptr) {
            p.first->p = nullptr;
        }
        return make_pair(p.first, v);
    }
    pair <vertex *, vertex *> p = split(v->r, key - cur_key - 1);
    v->r = p.first;
    upd(v);
    if (p.first != nullptr) {
        p.first->p = v;
    }
    v->p = nullptr;
    if (p.second != nullptr) {
        p.second->p = nullptr;
    }
    return make_pair(v, p.second);
};

vertex *merge(vertex *u, vertex *v) {
    if (u == nullptr) {
        return v;
    }
    if (v == nullptr) {
        return u;
    }
    if (u->priority > v->priority) {
        vertex *w = merge(u->r, v);
        u->r = w;
        upd(u);
        w->p = u;
        return u;
    }
    vertex *w = merge(u, v->l);
    v->l = w;
    upd(v);
    w->p = v;
    return v;
}

void insert_on_first_pos(vertex *v, int num) {
    while (v->l != nullptr) {
        v = v->l;
    }
    v->l = new vertex(num);
    v->l->p = v;
    edge_ptr[num][v->number] = v->l;
    while (v != nullptr) {
        upd(v);
        v = v->p;
    }
}

vertex *get_root(vertex *v) {
    while (v->p != nullptr) {
        v = v->p;
    }
    return v;
}

int get_key(vertex *v) {
    vector <vertex *> path;
    while (v != nullptr) {
        path.push_back(v);
        v = v->p;
    }
    int key = 0;
    for (int i = path.size() - 1; i > 0; --i) {
        if (path[i - 1] == path[i]->r) {
            key += (get_cnt(path[i]->l) + 1);
        }
    }
    key += get_cnt(path[0]->l);
    return key + 1;
}

void traversal(vertex *v) {
    if (v != nullptr) {
        traversal(v->l);
        cout << v->number + 1 << ' ';
        traversal(v->r);
    }
}

void link(int a, int b) {
    vertex *u = vertex_ptr[a];
    vertex *v = vertex_ptr[b];
    pair <vertex *, vertex *> A = split(get_root(u), get_key(u));
    if (A.second != nullptr) {
        insert_on_first_pos(A.second, a);
    } else {
        A.second = new vertex(a);
    }
    edge_ptr[a][b] = u;
    pair <vertex *, vertex *> B = split(get_root(v), get_key(v));
    if (B.second != nullptr) {
        insert_on_first_pos(B.second, b);
    } else {
        B.second = new vertex(b);
    }
    edge_ptr[b][a] = v;
    pair <vertex *, vertex *> p = split(B.first, 1);
    B.first = p.second;
    if (B.second != nullptr) {
        vertex *w = B.second;
        while (w->r != nullptr) {
            w = w->r;
        }
        vertex_ptr[p.first->number] = w;
        vertex *w1 = B.first;
        if (w1 != nullptr) {
            while (w1->l != nullptr) {
                w1 = w1->l;
            }
            edge_ptr[p.first->number][w1->number] = w;
        }
        if (p.first == v) {
            edge_ptr[b][a] = w;
        }
        delete p.first;
    }
    //cout << "after link " << a + 1 << ' ' << b + 1 << '\n';
    /*traversal(*/merge(merge(merge(A.first, B.second), B.first), A.second)/*)*/;
    //cout << "\n\n";
}

void cut(int a, int b) {
    vertex *u = edge_ptr[a][b];
    vertex *v = edge_ptr[b][a];
    int u_key = get_key(u);
    int v_key = get_key(v);
    if (u_key > v_key) {
        swap(u, v);
        swap(u_key, v_key);
    }
    pair <vertex *, vertex *> A = split(get_root(u), u_key);
    pair <vertex *, vertex *> B = split(A.second, get_key(v));
    pair <vertex *, vertex *> p = split(B.second, 1);
    B.second = p.second;
    if (p.first != nullptr) {
        vertex_ptr[p.first->number] = u;
        vertex *w1 = p.second;
        if (w1 != nullptr) {
            while (w1->l != nullptr) {
                w1 = w1->l;
            }
            edge_ptr[p.first->number][w1->number] = u;
        }
        delete p.first;
    }
    //cout << "after cut " << a + 1 << ' ' << b + 1 << '\n';
    /*traversal(*/merge(A.first, B.second)/*)*/;
    //cout << "\n";
    //traversal(B.first);
    //cout << "\n\n";
}

bool connected(int u, int v) {
    return get_root(vertex_ptr[u]) == get_root(vertex_ptr[v]);
}

int main() {
    cin >> n >> m;
    for (int i = 0; i < n; ++i) {
        vertex_ptr[i] = new vertex(i);
    }
    for (int i = 0; i < m; ++i) {
        string s;
        int u, v;
        cin >> s >> u >> v;
        --u;
        --v;
        if (s[0] == 'l') {
            link(u, v);
        } else if (s[1] == 'u') {
            cut(u, v);
        } else {
            cout << connected(u, v) << endl;
        }
    }
    return 0;
}