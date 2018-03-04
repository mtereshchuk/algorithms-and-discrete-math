#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include <queue>
#include <utility>
#include <climits>

typedef long long ll;

using namespace std;

const ll max_n = 50000, max_m = 150000, max_p = 150000;
ll n, m, p = 0, s, t, dual_a, dual_b, pos_s, pos_t, d[max_p], shift_v;
vector <ll> g[max_n], s_t_facet;
pair <ll, ll> edges[max_m];
unordered_map <ll, unordered_map <ll, ll>> weight, edge_to_what_facet, dual_weight;
unordered_map <ll, unordered_set <ll>> dual_g;

struct point {
    ll x, y;
};
point points[max_n];

double get_angle(ll a) {
    double angle;
    double x = points[a].x - points[shift_v].x;
    double y = points[a].y - points[shift_v].y;
    if (x > 0 && y >= 0) {
        angle = atan(y / x);
    } else if (x > 0 && y < 0) {
        angle = atan(y / x) + 2 * M_PI;
    } else if (x < 0) {
        angle = atan(y / x) + M_PI;
    } else if (x == 0 && y > 0) {
        angle = M_PI / 2;
    } else {
        angle = M_PI * 1.5;
    }
    return angle;
}

bool comparator(const ll &a, const ll &b) {
    double angle1 = get_angle(a), angle2 = get_angle(b);
    if (angle1 < angle2) {
        return 1;
    }
    return 0;
}

void data_input() {
    scanf("%lld%lld%lld%lld", &n, &m, &s, &t);
    --s;
    --t;
    for (ll i = 0; i < n; ++i) {
        scanf("%lld%lld", &points[i].x, &points[i].y);
    }
    for (ll i = 0; i < m; ++i) {
        ll u, v, w;
        scanf("%lld%lld%lld", &u, &v, &w);
        --u;
        --v;
        g[u].push_back(v);
        g[v].push_back(u);
        edges[i].first = u;
        edges[i].second = v;
        weight[u][v] = w;
        weight[v][u] = w;
    }
}

void sort_g() {
    for (ll i = 0; i < n; ++i) {
        shift_v = i;
        sort(g[i].begin(), g[i].end(), comparator);
    }
}

void find_facets() {
    unordered_map <ll, unordered_map <ll, bool>> used;
    vector <ll> facet;
    for (ll i = 0; i < n; ++i) {
        for (ll j = 0; j < g[i].size(); ++j) {
            ll u = i, v = g[i][j];
            facet.clear();
            bool flag1 = 0, flag2 = 0;
            while (!used[u][v]) {
                used[u][v] = 1;
                edge_to_what_facet[u][v] = p;
                if (v == s) {
                    flag1 = 1;
                }
                if (v == t) {
                    flag2 = 1;
                }
                facet.push_back(v);
                shift_v = v;
                auto it = upper_bound(g[v].begin(), g[v].end(), u, comparator);
                ll w;
                if (it != g[v].end()) {
                    w = *it;
                } else {
                    w = g[v][0];
                }
                u = v;
                v = w;
            }
            if (!facet.empty()) {
                if (flag1 && flag2) {
                    s_t_facet = move(facet);
                    dual_a = p;
                }
                p++;
            }
        }
    }
}

void add_new_facet() {
    for (ll i = 0; i < s_t_facet.size(); ++i) {
        if (s_t_facet[i] == s) {
            pos_s = i;
        }
        if (s_t_facet[i] == t) {
            pos_t = i;
        }
    }
    if (pos_s > pos_t) {
        swap(s, t);
        swap(pos_s, pos_t);
    }
    dual_b = p++;
    for (ll i = pos_s; i < pos_t; ++i) {
        ll u = s_t_facet[i];
        ll v = s_t_facet[i + 1];
        edge_to_what_facet[u][v] = dual_b;
    }
}

void build_dual_g() {
    for (ll i = 0; i < m; ++i) {
        ll u = edges[i].first;
        ll v = edges[i].second;
        ll f1 = edge_to_what_facet[u][v];
        ll f2 = edge_to_what_facet[v][u];
        if (f1 == f2) {
            continue;
        }
        if (f1 == dual_a && f2 == dual_b || f1 == dual_b && f2 == dual_a) {
            continue;
        }
        dual_g[f1].insert(f2);
        dual_g[f2].insert(f1);
        if (dual_weight.find(f1) != dual_weight.end() && dual_weight[f1].find(f2) != dual_weight[f1].end()) {
            dual_weight[f1][f2] = min(dual_weight[f1][f2], weight[u][v]);
            dual_weight[f2][f1] = dual_weight[f1][f2];
        } else {
            dual_weight[f1][f2] = weight[u][v];
            dual_weight[f2][f1] = dual_weight[f1][f2];
        }
    }
}

void find_shortest_paths() {
    unordered_set <ll> b;
    priority_queue <pair <ll, ll>, vector <pair <ll, ll>>, greater <pair <ll, ll>>> heap;
    for (ll i = 0; i < p; ++i) {
        d[i] = LONG_LONG_MAX;
        heap.push(make_pair(LONG_LONG_MAX, i));
        b.insert(i);
    }
    d[dual_a] = 0;
    for (auto i = dual_g[dual_a].begin(); i != dual_g[dual_a].end(); ++i) {
        ll weight = dual_weight[dual_a][*i];
        d[*i] =  weight;
        heap.push(make_pair(weight, *i));
    }
    while (!b.empty()) {
        ll min_d = heap.top().first;
        ll index_min_d = heap.top().second;
        heap.pop();
        if (b.find(index_min_d) != b.end()) {
            b.erase(index_min_d);
            for (auto j = dual_g[index_min_d].begin(); j != dual_g[index_min_d].end(); ++j) {
                if (b.find(*j) != b.end()) {
                    ll x = min_d + dual_weight[index_min_d][*j];
                    if (x < d[*j]) {
                        d[*j] = x;
                        heap.push(make_pair(x,*j));
                    }
                }
            }
        }
    }
}

void data_output() {
    printf("%lld", d[dual_b]);
}

int main() {
    data_input();
    sort_g();
    find_facets();
    add_new_facet();
    build_dual_g();
    find_shortest_paths();
    data_output();
    return 0;
}