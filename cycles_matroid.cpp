#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <unordered_set>

using namespace std;

typedef long long ll;

unsigned const int max_n = 20;
unsigned int n, m, w[max_n];
set <int> cycles;

void data_input() {
    freopen("cycles.in", "r", stdin);
    //freopen("in", "r", stdin);
    cin >> n >> m;
    for (int i = 0; i < n; ++i) {
        cin >> w[i];
    }
    int cur_size, cur_set;
    for (int i = 0; i < m; ++i) {
        cin >> cur_size;
        cur_set = 0;
        for (int j = 0; j < cur_size; ++j) {
            int e;
            cin >> e;
            cur_set += 1 << (e - 1);
        }
        cycles.insert(cur_set);
    }
}

set <int> independent_sets;

void find_independent_sets() {
    for (int i = 0; i < (1 << n); ++i) {
        bool dependent = 0;
        for (auto &c : cycles)  {
            if ((i & c) == c) {
                dependent = 1;
                break;
            }
        }
        if (!dependent) {
            independent_sets.insert(i);
        }
    }
}

int max_weight = 0;

void find_max_weight() {
    int max_size = 0;
    for (auto &s : independent_sets) {
        int cur_size = 0;
        for (int i = 0; i < n; ++i) {
            if (s & (1 << i)) {
                ++cur_size;
            }
        }
        max_size = max(max_size, cur_size);
    }
    for (auto &s : independent_sets) {
        int cur_size = 0, cur_weight = 0;
        for (int i = 0; i < n; ++i) {
            if (s & (1 << i)) {
                ++cur_size;
                cur_weight += w[i];
            }
        }
        if (cur_size == max_size) {
            max_weight = max(max_weight, cur_weight);
        }
    }
}

void data_output() {
    freopen("cycles.out", "w", stdout);
    cout << max_weight;
}

int main() {
    ios_base::sync_with_stdio(0);
    data_input();
    find_independent_sets();
    find_max_weight();
    data_output();
    return 0;
}