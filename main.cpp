#include <bits/stdc++.h>
using namespace std;
#define gc getchar

inline int read()
{
    int value = 0;
    bool ne = 0;
    char c = gc();
    while (c == ' ' or c == '\n')
        c = gc();
    if (c == '-')
    {
        ne = 1;
        c = gc();
    }
    while (c >= '0' and c <= '9')
    {
        value = (value << 3) + (value << 1) + c - '0';
        c = gc();
    }
    if (ne)
        value *= -1;
    return value;
}
inline void write(int x)
{
    if (x > 9)
        write(x / 10);
    putchar(x % 10 + '0');
}
int main()
{
    int tt;
    tt = 1;
    while (tt--)
    {
        int n = read();
        vector<int> a(n);
        long long sum = 0;
        bool zeros = false;
        vector<int> odds;
        vector<int> ones;
        for (int i = 0; i < n; i++)
        {
            a[i] = read();
            sum += a[i];
            zeros |= (a[i] == 0);
            if (a[i] == 1)
            {
                ones.push_back(i);
            }
            else
            {
                if (a[i] % 2 == 1)
                {
                    odds.push_back(i);
                }
            }
        }
        int m = (int)sum / 2;
        int cycles = m - (n - 1);
        int vers = n - (int)max((odds.size() + ones.size()) / 2, ones.size());
        if (sum % 2 != 0 || sum > 10 * n || cycles < 0 || cycles > (vers - 1) / 2 || (n > 1 && zeros))
        {
            puts("-1");
            continue;
        }
        vector<int> init_a = a;
        vector<vector<int>> g(n);
        int cnt = 0;
        auto Add = [&](int x, int y)
        {
            g[x].push_back(cnt);
            g[y].push_back(cnt);
            cnt += 1;
        };
        if (cycles == 0)
        {
            set<pair<int, int>> s;
            for (int i = 0; i < n; i++)
            {
                s.emplace(a[i], i);
            }
            for (int i = 0; i < n - 1; i++)
            {
                int x = s.begin()->second;
                s.erase(s.begin());
                int y = prev(s.end())->second;
                s.erase(prev(s.end()));
                Add(x, y);
                a[y] -= 1;
                s.emplace(a[y], y);
            }
        }
        else
        {
            odds.insert(odds.end(), ones.begin(), ones.end());
            vector<vector<int>> cut(n);
            vector<bool> alive(n, true);
            int sz = (int)odds.size();
            int ptr = 0;
            for (int i = 0; i < sz - 1 - i; i++)
            {
                int x = odds[i];
                int y = odds[sz - 1 - i];
                if (a[x] > 1)
                {
                    a[x] += a[y] - 2;
                    cut[x].push_back(y);
                    alive[y] = false;
                }
                else
                {
                    while (ptr < n && (!alive[ptr] || a[ptr] < 4))
                    {
                        ++ptr;
                    }
                    a[ptr] -= 2;
                    cut[ptr].push_back(x);
                    cut[ptr].push_back(y);
                    alive[x] = false;
                    alive[y] = false;
                }
            }
            vector<int> order;
            for (int i = 0; i < n; i++)
            {
                if (alive[i])
                {
                    order.push_back(i);
                }
            }
            sort(order.begin(), order.end(), [&](int i, int j)
                 { return a[i] > a[j]; });
            vector<int> que(a[order[0]] / 2, order[0]);
            ptr = 1;
            int b = 0;
            for (int i = 0; i < cycles; i++)
            {
                int x = que[b++];
                int y = order[ptr++];
                int z = order[ptr++];
                Add(x, y);
                Add(y, z);
                Add(z, x);
                a[x] -= 2;
                a[y] -= 2;
                a[z] -= 2;
                for (int j = 0; j < a[y] / 2; j++)
                {
                    que.push_back(y);
                }
                for (int j = 0; j < a[z] / 2; j++)
                {
                    que.push_back(z);
                }
            }
            for (int i = 0; i < n; i++)
            {
                for (int j : cut[i])
                {
                    g[j] = vector<int>(g[i].end() - (a[j] - 1), g[i].end());
                    g[i].erase(g[i].end() - (a[j] - 1), g[i].end());
                }
            }
            for (int i = 0; i < n; i++)
            {
                for (int j : cut[i])
                {
                    Add(i, j);
                }
            }
            int any = order[0];
            while (ptr < (int)order.size())
            {
                int i = order[ptr++];
                g[i].push_back(g[any][0]);
                g[i].push_back(cnt);
                g[any][0] = cnt;
                cnt++;
            }
        }
        vector<pair<int, int>> edges(m, make_pair(-1, -1));
        for (int i = 0; i < n; i++)
        {
            for (int j : g[i])
            {
                if (edges[j].first == -1)
                {
                    edges[j].first = i;
                }
                else
                {
                    edges[j].second = i;
                }
            }
        }
        write(m);
        putchar('\n');
        for (int i = 0; i < m; i++)
        {
            putchar('2');
            putchar(' ');
            write(edges[i].first + 1);
            putchar(' ');
            write(edges[i].second + 1);
            putchar('\n');
        }
    }
    return 0;
}