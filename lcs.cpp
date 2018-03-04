#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <string>

using namespace std;

typedef long long ll;

const int /*max_size = 100000,*/ p = 31;
string s, t;
int max_string_size, cur_sz;
//ll p_pow[max_size], h_s[max_size], h_t[max_size];
vector <ll> p_pow, h_s, h_t;
unordered_set <int> answer;

void data_input() {
    cin >> s >> t;
    max_string_size = max(s.size(), t.size());
    p_pow.resize(max_string_size);
    h_s.resize(s.size());
    h_t.resize(t.size());
}

void find_p_pows() {
    p_pow[0] = 1;
    for (int i = 1; i < max_string_size; ++i) {
        p_pow[i] = p_pow[i - 1] * p;
    }
}

void find_hashes() {
    for (int i = 0; i < s.size(); ++i) {
        h_s[i] = (s[i] - '`') * p_pow[i];
        if (i) h_s[i] += h_s[i - 1];
    }
    for (int i = 0; i < t.size(); ++i) {
        h_t[i] = (t[i] - '`') * p_pow[i];
        if (i) h_t[i] += h_t[i - 1];
    }
}

unordered_set <int> res;
unordered_set <ll> s_hashes;

bool substring_exist(int sz) {
    res.clear();
    s_hashes.clear();
    for (int i = 0; i + sz < s.size() + 1; ++i) {
        ll h = h_s[i + sz - 1];
        if (i) h -= h_s[i - 1];
        h *= p_pow[max_string_size - i - 1];
        s_hashes.insert(h);
    }
    for (int i = 0; i + sz < t.size() + 1; ++i) {
        ll h = h_t[i + sz - 1];
        if (i) h -= h_t[i - 1];
        h *= p_pow[max_string_size - i - 1];
        if (s_hashes.find(h) != s_hashes.end()) {
            res.insert(i);
        }
    }
    if (!res.empty()) {
        cur_sz = sz;
        answer = move(res);
        return 1;
    }
    return 0;
}

void bin_search() {
    int l = -1, r = max_string_size + 1;
    while (l < r - 1) {
        int m = (l + r) / 2;
        if (substring_exist(m)) l = m;
        else r = m;
    }
}

bool is_less(int a, int b) {
    while (a + cur_sz < t.size() + 1) {
        if (t[a] < t[b]) {
            return 1;
        }
        ++a;
        ++b;
    }
    return 0;
}

int get_min() {
    int res = *answer.begin();
    for (auto i : answer) {
        if (is_less(i, res)) {
            res = i;
        }
    }
    return res;
}

void data_output() {
    if (!answer.empty()) {
        int start_index = get_min();
        for (int i = 0; i < cur_sz; ++i) {
            cout << t[start_index + i];
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    freopen("common.in", "r", stdin);
    //freopen("in", "r", stdin);
    freopen("common.out", "w", stdout);
    data_input();
    find_p_pows();
    find_hashes();
    bin_search();
    data_output();
    return 0;
}