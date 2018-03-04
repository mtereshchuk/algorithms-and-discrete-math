#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <tuple>

using namespace std;

size_t alphabet_size = 11;
string s;

void data_input() {
    //freopen("in", "r", stdin);
    freopen("refrain.in", "r", stdin);
    //cin >> s;
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < n; ++i) {
        int a;
        cin >> a;
        s.push_back((char) ('`' + a));
    }
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
    /*for (int i = 0; i < s.size(); ++i) {
        cout << suffix_array[i] << ' ';
    }
    cout << '\n';
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
        /*for (int i = 0; i < s.size(); ++i) {
            cout << suffix_array[i] << ' ';
        }
        cout << '\n';
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
            lcp[suffix_array_inverse[i]] = cur_lcp;
        }
    }
}

long long max_res, refrain_size, suffix_index;

void find_refrain() {
    vector <tuple <int, long long, int>> lcp_cnt_index;
    max_res = s.size() - 1;
    for (int i = 1; i < s.size(); ++i) {
        long long cur_cnt_of_suffixes = 1;
        while (!lcp_cnt_index.empty() && lcp[i] <= get <0>(lcp_cnt_index.back())) {
            tuple <int, long long, int> cur_tuple = move(lcp_cnt_index.back());
            lcp_cnt_index.pop_back();
            cur_cnt_of_suffixes += get <1>(cur_tuple);
            long long cur_res = cur_cnt_of_suffixes * get <0>(cur_tuple);
            if (cur_res > max_res) {
                max_res = cur_res;
                refrain_size = get <0>(cur_tuple);
                suffix_index = get <2>(cur_tuple);
            }
        }
        lcp_cnt_index.emplace_back(lcp[i], cur_cnt_of_suffixes, i);
    }
}

void data_output() {
    freopen("refrain.out", "w", stdout);
    /*for (int i = 1; i < s.size(); ++i) {
        cout << suffix_array[i] << ' ';
    }
    cout << '\n';
    for (int i = 1; i < s.size() - 1; ++i) {
        cout << lcp[i] << ' ';
    }*/
    if (max_res > s.size() - 1) {
        cout << max_res << '\n' << refrain_size << '\n';// << suffix_index;
        for (int i = suffix_array[suffix_index]; i < suffix_array[suffix_index] + refrain_size; ++i) {
            if (s[i] != '`') {
                cout << (int) (s[i] - '`') << ' ';
            }
        }
    } else {
        cout << max_res << '\n' << max_res << '\n';
        for (auto &c : s) {
            if (c != '`') {
                cout << (int) (c - '`') << ' ';
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    data_input();
    do_initialization();
    find_suffix_array();
    find_lcp();
    find_refrain();
    data_output();
    return 0;
}