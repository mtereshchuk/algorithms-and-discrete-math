#include <iostream>
#include <vector>

using namespace std;

const size_t alphabet_size = 26, max_number_of_strings = 1000000;
unsigned int n;
string t;

struct state{
    vector <state *> next_state, transition;
    state *par, *suffix_link, *nearest_terminal;
    char par_cur_symbol;
    bool terminal, visited;
    vector <int> strings_numbers;
    state(state *p, char c) {
        next_state.assign(alphabet_size, nullptr);
        transition.assign(alphabet_size, nullptr);
        par = p;
        suffix_link = nullptr;
        nearest_terminal = nullptr;
        par_cur_symbol = c;
        terminal = false;
        visited = false;
    }
};

state *root;

state *get_transition(state *st, char c);

state *get_suffix_link(state *st) {
    if (!st->suffix_link) {
        if (st == root || st->par == root) {
            st->suffix_link = root;
        } else {
            st->suffix_link = get_transition(get_suffix_link(st->par), st->par_cur_symbol);
        }
    }
    return st->suffix_link;
}

state *get_transition(state *st, char c) {
    int c_index = c - 'a';
    if (!st->transition[c_index]) {
        if (st->next_state[c_index]) {
            st->transition[c_index] = st->next_state[c_index];
        } else if (st == root) {
            st->transition[c_index] = root;
        } else {
            st->transition[c_index] = get_transition(get_suffix_link(st), c);
        }
    }
    return st->transition[c_index];
}

state *get_nearest_terminal(state *st) {
    if (!st->nearest_terminal) {
        if (get_suffix_link(st)->terminal) {
            st->nearest_terminal = get_suffix_link(st);
        } else if (get_suffix_link(st) == root) {
            st->nearest_terminal = root;
        } else {
            st->nearest_terminal = get_nearest_terminal(get_suffix_link(st));
        }
    }
    return st->nearest_terminal;
}

void add_string(string s, int s_number) {
    state *cur_state = root;
    for (auto &c : s) {
        int c_index = c - 'a';
        if (!cur_state->next_state[c_index]) {
            cur_state->next_state[c_index] = new state(cur_state, c);
        }
        cur_state = cur_state->next_state[c_index];
    }
    cur_state->terminal = true;
    cur_state->strings_numbers.push_back(s_number);
}

bool in_text[max_number_of_strings];

void find_strings_in_text() {
    state *cur_state = root;
    for (auto &c : t) {
        cur_state = get_transition(cur_state, c);
        if (!cur_state->visited) {
            state *st = cur_state;
            while (st != root) {
                if (!st->strings_numbers.empty()) {
                    for (auto &i : st->strings_numbers) {
                        in_text[i] = true;
                    }
                }
                st = get_nearest_terminal(st);
            }
        }
        cur_state->visited = true;
    }
}

void data_output() {
    freopen("search4.out", "w", stdout);
    for (int i = 0; i < n; ++i) {
        if (in_text[i]) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    freopen("search4.in", "r", stdin);
    //freopen("in", "r", stdin);
    root = new state(nullptr, ' ');
    cin >> n;
    string s;
    for (int i = 0; i < n; ++i) {
        cin >> s;
        add_string(s, i);
    }
    cin >> t;
    find_strings_in_text();
    data_output();
    return 0;
}
