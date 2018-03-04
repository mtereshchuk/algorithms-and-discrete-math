#include <iostream>
#include <cmath>

using namespace std;

int n, m, p[50000], d[50000], dp[50000][16], queries[50000][2];

void dataInput() {
    FILE *inFile;
    inFile = fopen("lca.in","r");
    fscanf(inFile, "%d", &n);
    for (int i = 1; i < n; ++i) {
        int v;
        fscanf(inFile, "%d", &v);
        p[i] = v - 1;
        d[i] = d[p[i]] + 1;
    }
    fscanf(inFile, "%d", &m);
    for (int i = 0; i < m; ++i) {
        int u, v;
        fscanf(inFile, "%d%d", &u, &v);
        queries[i][0] = u - 1;
        queries[i][1] = v - 1;
    }
    fclose(inFile);
}

void dynamics_filling() {
    for (int i = 0; i < n; ++i) {
        dp[i][0] = p[i];
    }
    for (int i = 1; i < floor(log2(n)) + 1; ++i) {
        for (int v = 1; v < n; ++v) {
            dp[v][i] = dp[dp[v][i - 1]][i - 1];
        }
    }
}

int get_lca(int u, int v) {
    if (d[u] > d[v]) {
        swap(u, v);
    }
    for (int i = (int) floor(log2(n)); i >= 0; --i) {
        if (d[v] - d[u] >= (1 << i)) {
            v = dp[v][i];
        }
    }
    if (u == v) {
        return u;
    }
    for (int i = (int) floor(log2(n)); i >= 0; --i) {
        if (dp[u][i] != dp[v][i]) {
            u = dp[u][i];
            v = dp[v][i];
        }
    }
    return p[u];
}

void dataOutput() {
    FILE *outFile;
    outFile = fopen ("lca.out","w");
    for (int i = 0; i < m; ++i) {
        fprintf(outFile, "%d\n", get_lca(queries[i][0], queries[i][1]) + 1);
    }
    fclose(outFile);
}

int main() {
    p[0] = 0;
    d[0] = 0;
    dataInput();
    dynamics_filling();
    dataOutput();
    return 0;
}