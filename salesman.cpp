#include <cstdio>
#include <algorithm>

using namespace std;

const long long impossibleValue = 15300000100;
int n, m, d[20][20];
long long s[20][270000], result;

void dataInput()
{
    freopen("salesman.in", "r", stdin);
    cin >> n >> m;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (i != j)
            {
                d[i][j] = -1;
            }
            else
            {
                d[i][j] = 0;
            }
        }
    }
    for (int i = 0; i < m; ++i)
    {
        int a, b, w;
        cin >> a >> b >> w;
        d[a - 1][b - 1] = w;
        d[b - 1][a - 1] = w;
    }
    fclose(stdin);
}

void sFilling()
{
    long long allVariantsOfWays = 1 << n;
    for (long long z = 0; z < allVariantsOfWays; ++z)
    {
        for (int i = 0; i < n; ++i)
        {
            if (!z)
            {
                s[i][z] = 0;
            }
            else
            {
                s[i][z] = impossibleValue;
            }
        }
    }
    for (long long z = 0; z < allVariantsOfWays; ++z)
    {
        for (int i = 0; i < n; ++i)
        {
            if (z & (1 << i))
            {
                for (int j = 0; j < n; ++j)
                {
                    if (d[j][i] != -1)
                    {
                        long long x = z - (1 << i);
                        s[i][z] = min(s[i][z], d[j][i] + s[j][x]);
                    }
                }
            }
        }
    }
    long long AllTheWay = (1 << n) - 1;
    result = impossibleValue;
    for (int i = 0; i < n; ++i)
    {
        result = min(result, s[i][AllTheWay]);
    }
}

void dataOutput()
{
    freopen("salesman.out", "w", stdout);
    if (result != impossibleValue)
    {
        cout << result;
    }
    else
    {
        cout << -1;
    }
    fclose(stdout);
}

int main()
{
    dataInput();
    sFilling();
    dataOutput();
    return 0;
}
