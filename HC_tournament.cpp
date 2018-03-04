#include <iostream>
#include <ctime>
#include <vector>

using namespace std;

int n;
vector <int> cycle(3), not_cycle;
bool matrix[1000][1000];

void dataInput() {
    FILE *inFile;
    inFile = fopen("guyaury.in","r");
    fscanf(inFile, "%d", &n);
    char s[4000];
    for (int i = 1; i < n; ++i) {
        fscanf(inFile, "%s", s);
        for (int j = 0; j < i; ++j) {
            matrix[i][j] = s[j] - '0';
            matrix[j][i] = !(s[j] - '0');
        }
    }
    fclose(inFile);
}

void find_HC() {
    for (int i = 3; i < n; ++i) {
        int x_index = -1, a_index, b_index;
        for (int j = 0; j < not_cycle.size(); ++j) {
            a_index = -1;
            b_index = -1;
            for (int k = 0; k < cycle.size(); ++k) {
                if (!matrix[not_cycle[j]][cycle[k]]) {
                    a_index = k;
                    break;
                }
            }
            if (a_index != -1) {
                for (int k = a_index + 1; k < a_index + cycle.size(); ++k) {
                    if (matrix[not_cycle[j]][cycle[k % cycle.size()]]) {
                        b_index = k % cycle.size();
                        break;
                    }
                }
            }
            if (a_index != -1 && b_index != -1) {
                x_index = j;
                break;
            }
        }
        if (x_index != -1) {
            cycle.insert(cycle.begin() + b_index, not_cycle[x_index]);
            not_cycle.erase(not_cycle.begin() + x_index);
        } else {
            int c_index, d_index;
            for (int j = 0; j < not_cycle.size(); ++j) {
                c_index = -1;
                d_index = -1;
                if (matrix[not_cycle[j]][cycle[0]]) {
                    for (int k = 0; k < not_cycle.size(); ++k) {
                        if (j != k) {
                            if (matrix[not_cycle[k]][not_cycle[j]] && matrix[cycle[0]][not_cycle[k]]) {
                                c_index = j;
                                d_index = k;
                                break;
                            }
                        }
                    }
                }
                if (c_index != -1 && d_index != -1) {
                    break;
                }
            }
            not_cycle.push_back(cycle[cycle.size() - 1]);
            cycle.pop_back();
            cycle.push_back(not_cycle[d_index]);
            cycle.push_back(not_cycle[c_index]);
            if (c_index < d_index) {
                not_cycle.erase(not_cycle.begin() + d_index);
                not_cycle.erase(not_cycle.begin() + c_index);
            } else {
                not_cycle.erase(not_cycle.begin() + c_index);
                not_cycle.erase(not_cycle.begin() + d_index);
            }
        }
    }
}

void find_HC_3() {
    srand((unsigned int) time(0));
    int x = rand() % n;
    int a = -1, b = -1;
    for (int i = 0; i < n; ++i) {
        if (i != x) {
            if (matrix[i][x]) {
                for (int j = 0; j < n; ++j) {
                    if (i != j) {
                        if (matrix[j][i] && matrix[x][j]) {
                            a = i;
                            b = j;
                            break;
                        }
                    }
                }
            }
        }
        if (a != -1 && b != -1) {
            break;
        }
    }
    cycle[0] = x;
    cycle[1] = b;
    cycle[2] = a;
    for (int i = 0; i < n; ++i) {
        if (i != x && i != b && i != a) {
            not_cycle.push_back(i);
        }
    }
}

void dataOutput() {
    FILE *outFile;
    outFile = fopen ("guyaury.out","w");
    if (n >= 3) {
        for (int i = 0; i < cycle.size(); ++i) {
            fprintf(outFile, "%d ", cycle[i] + 1);
        }
    } else {
        fprintf(outFile, "1");
    }
    fclose(outFile);
}

int main() {
    dataInput();
    if (n >= 3) {
        find_HC_3();
        find_HC();
    }
    dataOutput();
    return 0;
}