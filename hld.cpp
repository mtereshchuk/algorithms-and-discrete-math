#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <cmath>

using namespace std;

int n, m, p[100000], d[100000], on_what_path[100000], on_what_pos[100000], size[100000], root_of_path[100000], what_path[100000];
unordered_map <int, unordered_set <int>> g;
unordered_set <int> no_heavy_edges;
unordered_map <int, unordered_set <int>> heavy_edges;
vector <vector <int>> paths, trees_val;
vector <int> a_trees_sizes;
unordered_map <int, unordered_map <int, int>> weight;

void dfs1(int u) {
    size[u] = 1;
    no_heavy_edges.insert(u);
    on_what_path[u] = -1;
    for (auto v : g[u]) {
        if (v != p[u]) {
            p[v] = u;
            d[v] = d[u] + 1;
            dfs1(v);
            size[u] += size[v];
        }
    }
    for (auto v : g[u]) {
        if (v != p[u] && (size[v] << 1) >= size[u]) {
            heavy_edges[u].insert(v);
            heavy_edges[v].insert(u);
            no_heavy_edges.erase(u);
            break;
        }
    }
}

void dfs2(int u) {
    for (auto v : g[u]) {
        if (v != p[u]) {
            if (size[v] * 2 >= size[u]) {
                root_of_path[v] = root_of_path[u];
                what_path[v] = what_path[u];
            } else {
                root_of_path[v] = u;
                what_path[v] = v;
            }
            dfs2(v);
        }
    }
}

void build_trees() {
    trees_val.resize(paths.size());
    for (int i = 0; i < paths.size(); ++i) {
        int a_size = 1 << (int) ceil(log2(paths[i].size() - 1));
        a_trees_sizes.push_back(a_size);
        unsigned int tree_size = (unsigned int) a_size * 2 - 1;
        trees_val[i].resize(tree_size);
    }
}

void upd(int num_of_tree, int u, int l, int r, int L, int R) {
    if (l >= R || r <= L) {
        return;
    }
    if (l >= L && r <= R) {
        trees_val[num_of_tree][u]++;
        return;
    }
    int m = (l + r) / 2;
    upd(num_of_tree, u * 2 + 1, l, m, L, R);
    upd(num_of_tree, u * 2 + 2, m, r, L, R);
}

int get_val(int num_of_tree, int u, int l, int r, int L, int R) {
    if (l >= R || r <= L) {
        return 0;
    }
    if (l >= L && r <= R) {
        return trees_val[num_of_tree][u];
    }
    int m = (l + r) / 2;
    return trees_val[num_of_tree][u] + get_val(num_of_tree, u * 2 + 1, l, m, L, R) + get_val(num_of_tree, u * 2 + 2, m, r, L, R);
}

void build_HLD() {
    p[0] = 0;
    d[0] = 0;
    dfs1(0);
    root_of_path[0] = 0;
    what_path[0] = 0;
    dfs2(0);
    for (auto u : no_heavy_edges) {
        int v = u;
        vector <int> path;
        while (v && heavy_edges.find(v) != heavy_edges.end() && heavy_edges[v].find(p[v]) != heavy_edges[v].end()) {
            on_what_pos[v] = path.size();
            path.push_back(v);
            on_what_path[v] = paths.size();
            v = p[v];
        }
        if (!path.empty()) {
            on_what_pos[v] = path.size();
            path.push_back(v);
            on_what_path[v] = paths.size();
            paths.push_back(path);
        }
    }
    build_trees();
}

int get_lca(int u, int v) {
    if (what_path[u] == what_path[v]) {
        if (d[u] > d[v]) {
            return v;
        }
        return u;
    }
    if (d[root_of_path[u]] > d[root_of_path[v]]) {
        return get_lca(root_of_path[u], v);
    }
    if (d[root_of_path[u]] == d[root_of_path[v]]) {
        if (d[u] > d[v]) {
            return get_lca(root_of_path[u], v);
        }
        return get_lca(u, root_of_path[v]);
    }
    return get_lca(u, root_of_path[v]);
}

void add(int u, int lca) {
    while (u != lca) {
        if (heavy_edges.find(u) == heavy_edges.end() || heavy_edges[u].find(p[u]) == heavy_edges[u].end()) {
            if (weight.find(u) != weight.end() && weight[u].find(p[u]) != weight[u].end()) {
                weight[u][p[u]]++;
                weight[p[u]][u]++;
            } else {
                weight[u][p[u]] = 1;
                weight[p[u]][u] = 1;
            }
            u = p[u];
        } else {
            if (on_what_path[u] == on_what_path[lca]) {
                upd(on_what_path[u], 0, 0, a_trees_sizes[on_what_path[u]], on_what_pos[u], on_what_pos[lca]);
                break;
            } else {
                upd(on_what_path[u], 0, 0, a_trees_sizes[on_what_path[u]], on_what_pos[u], a_trees_sizes[on_what_path[u]]);
                u = paths[on_what_path[u]][paths[on_what_path[u]].size() - 1];
            }
        }
    }
}

void P(int u, int v) {
    int lca = get_lca(u, v);
    add(u, lca);
    add(v, lca);
}

void Q(int u, int v) {
    if (heavy_edges.find(u) == heavy_edges.end() || heavy_edges[u].find(v) == heavy_edges[u].end()) {
        if (weight.find(u) != weight.end() && weight[u].find(v) != weight[u].end()) {
            printf("%d\n", weight[u][v]);
        } else {
            printf("0\n");
        }
    } else {
        if (u == p[v]) {
            printf("%d\n", get_val(on_what_path[u], 0, 0, a_trees_sizes[on_what_path[u]], on_what_pos[v], on_what_pos[v] + 1));
        } else {
            printf("%d\n", get_val(on_what_path[u], 0, 0, a_trees_sizes[on_what_path[u]], on_what_pos[u], on_what_pos[u] + 1));
        }
    }
}

int main() {
    scanf("%d%d", &n, &m);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        scanf("%d%d", &u, &v);
        --u;
        --v;
        g[u].insert(v);
        g[v].insert(u);
    }
    build_HLD();
    char c;
    scanf("%c", &c);
    for (int i = 0; i < m; ++i) {
        scanf("%c", &c);
        int u, v;
        scanf("%d%d", &u, &v);
        --u;
        --v;
        if (c == 'P') {
            P(u, v);
        } else {
            Q(u, v);
        }
        scanf("%c", &c);
    }
    fclose(stdin);
    return 0;
}