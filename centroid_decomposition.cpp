#include <iostream>
#include <unordered_set>
#include <unordered_map>

using namespace std;

const int start_vertex_of_max = 200000;
int n, p[200000], size[200000], p_in_decomposition[200000];
unordered_map <int, unordered_set <int>> g;
unordered_set <int> used;

void data_input() {
    scanf("%d", &n);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        scanf("%d%d", &u, &v);
        --u;
        --v;
        g[u].insert(v);
        g[v].insert(u);
    }
}

void dfs(int u) {
    size[u] = 1;
    for (auto v : g[u]) {
        if (v != p[u] && used.find(v) == used.end()) {
            p[v] = u;
            dfs(v);
            size[u] += size[v];
        }
    }
}

void find_centroid(int u, int &centroid, int sz) {
    for (auto v : g[u]) {
        if (used.find(v) == used.end() && v != p[u]) {
            find_centroid(v, centroid, sz);
        }
    }
    if (centroid == -1 && 2 * size[u] >= sz) {
        centroid = u;
    }
}

int build_centroid_decomposition(int u) {
    p[u] = -1;
    dfs(u);
    int cur_centroid = -1;
    find_centroid(u, cur_centroid, size[u]);
    used.insert(cur_centroid);
    for (auto v : g[cur_centroid]) {
        if (used.find(v) == used.end()) {
            p_in_decomposition[build_centroid_decomposition(v)] = cur_centroid;
        }
    }
    return cur_centroid;
}

void data_output() {
    for (int i = 0; i < n; ++i) {
        printf("%d ", p_in_decomposition[i] + 1);
    }
}

int main() {
    data_input();
    p_in_decomposition[build_centroid_decomposition(0)] = -1;
    data_output();
    return 0;
}