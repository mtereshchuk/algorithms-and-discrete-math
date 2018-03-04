#include <iostream>
#include <vector>

using namespace std;

const size_t alphabet_size = 27;
string s;

void data_input() {
    freopen("in", "r", stdin);
    //freopen("array.in", "r", stdin);
    cin >> s;
}

size_t cur_eq_class;
vector <int> cnt, suffix_array, eq_class;

void do_initialization() {
    s += '`';
    cnt.assign(alphabet_size, 0);
    for (auto &c : s) {
        ++cnt[c - '`'];
    }
    for (int i = 1; i < cnt.size(); ++i) {
        cnt[i] += cnt[i - 1];
    }
    suffix_array.resize(s.size());
    for (int i = 0; i < s.size(); ++i) {
        suffix_array[--cnt[s[i] - '`']] = i;
    }
    eq_class.resize(s.size());
    cur_eq_class = 0;
    char last_symbol = '_';
    for (auto &suffix : suffix_array) {
        if (s[suffix] != last_symbol) {
            last_symbol = s[suffix];
            ++cur_eq_class;
        }
        eq_class[suffix] = cur_eq_class - 1;
    }
    for (int i = 0; i < s.size(); ++i) {
        cout << suffix_array[i] << ' ';
    }
    cout << '\n';/*
    for (int i = 0; i < s.size(); ++i) {
        cout << eq_class[i] << ' ';
    }
    cout << '\n';*/
}

void find_suffix_array() {
    vector <int> suffix_help(s.size());
    vector <int> eq_class_help(s.size());
    for (int cur_size = 1; cur_size < s.size(); cur_size *= 2) {
        for (int i = 0; i < s.size(); ++i) {
            suffix_help[i] = (suffix_array[i] + s.size() - cur_size) % s.size();
        }
        cnt.assign(cur_eq_class, 0);
        for (auto &h : suffix_help) {
            ++cnt[eq_class[h]];
        }
        for (int i = 1; i < cnt.size(); ++i) {
            cnt[i] += cnt[i - 1];
        }
        for (int i = s.size() - 1; i >= 0; --i) {
            suffix_array[--cnt[eq_class[suffix_help[i]]]] = suffix_help[i];
        }
        cur_eq_class = 1;
        for (int i = 1; i < s.size(); ++i) {
            int a = (suffix_array[i] + cur_size) % s.size();
            int b = (suffix_array[i - 1] + cur_size) % s.size();
            if (eq_class[suffix_array[i]] != eq_class[suffix_array[i - 1]]
                || eq_class[a] != eq_class[b]) {
                ++cur_eq_class;
            }
            eq_class_help[suffix_array[i]] = cur_eq_class - 1;
        }
        eq_class = eq_class_help;
        for (int i = 0; i < s.size(); ++i) {
            cout << suffix_array[i] << ' ';
        }
        cout << '\n';/*
        for (int i = 0; i < s.size(); ++i) {
            cout << eq_class[i] << ' ';
        }
        cout << '\n';*/
    }
}

vector <int> lcp;

void find_lcp() {
    lcp.resize(s.size());
    vector <int> suffix_array_inverse(s.size());
    for (int i = 0; i < s.size(); ++i) {
        suffix_array_inverse[suffix_array[i]] = i;
    }
    int cur_lcp = 0;
    for (int i = 0; i < s.size(); ++i) {
        if (cur_lcp > 0) {
            --cur_lcp;
        }
        if (suffix_array_inverse[i] < s.size() - 1) {
            int prefix = suffix_array[suffix_array_inverse[i] + 1];
            while (i + cur_lcp < s.size() && prefix + cur_lcp < s.size()
                   && s[i + cur_lcp] == s[prefix + cur_lcp]) {
                ++cur_lcp;
            }
            lcp[suffix_array_inverse[i]] = cur_lcp;
        } else {
            cur_lcp = 0;
        }
    }
}

void data_output() {
    //freopen("array.out", "w", stdout);
    for (int i = 1; i < s.size(); ++i) {
        cout << suffix_array[i] + 1 << ' ';
    }
    cout << '\n';
    for (int i = 1; i < s.size() - 1; ++i) {
        cout << lcp[i] << ' ';
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    data_input();
    do_initialization();
    find_suffix_array();
    find_lcp();
    data_output();
    return 0;
}