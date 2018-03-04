#include <iostream>

using namespace std;

string s;
int p[1000000];

void data_input() {
    cin >> s;
}

void get_prefix_function() {
    p[0] = 0;
    for (int i = 1; i < s.size(); ++i) {
        int a = p[i - 1];
        while (a && s[a] != s[i]) a = p[a - 1];
        if (s[a] == s[i]) ++a;
        p[i] = a;
    }
}

void data_output() {
    for (int i = 0; i < s.size(); ++i) {
        cout << p[i] << ' ';
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    data_input();
    get_prefix_function();
    data_output();
    return 0;
}