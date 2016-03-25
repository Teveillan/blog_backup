title: "HDU 5296 Annoying problem"
date: 2015-12-16 22:39:30
categories: 红名之路
tags: [HDU]
description: 
---

最初以为是DP，结果发现并不是。= =

题意：在一棵树上，边有权重。有两种操作：
1. 如果v不在集合中，那么将点v放入到集合
2. 如果v在集合中，那么将v移出集合
求每次操作后，选择一些边将集合中的点连在一起，问边权和为多少。

先初始化dfs序，以及跟节点到点x的距离dis。每当我们要插入一个点x，找到在集合中且dfs序比x第一个大的点u，以及第一个小的点v。则新增的边的边权为
$$dis[x]−dis[lca(x,u)]−dis[lca(x,v)]+dis[lca(u,v)]$$

<!--more-->

如果没有dfs序比v大或者比v小的点，则u v分别取集合中dfs序最大和最小的。

```c++
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <iostream>
#include <map>

using namespace std;

#define pb push_back

#define REP(i,n) for(int i=0;i<(n);++i)
#define FOR(i,l,h) for(int i=(l);i<=(h);++i)
#define DWN(i,h,l) for(int i=(h);i>=(l);--i)

typedef pair<int,int> pii;

const int N = 1e5 + 10;
vector<pii> head[N];
bool visit[N];
int dfn[N], deep[N], dis[N], fa[N][20];
void dfs(int cur, int pre, int &t) { //初始化dfs序，以及fa(在线求lca)
    dfn[cur] = ++t;
    deep[cur] = deep[pre] + 1;
    fa[cur][0] = pre;
    FOR(i, 1, 19) fa[cur][i] = fa[ fa[cur][i-1] ][i-1];
    REP(i, head[cur].size()) {
        int to = head[cur][i].first, c = head[cur][i].second;
        if(to != pre) {
            dis[to] = dis[cur] + c;
            dfs(to, cur, t);
        }
    }
}

int lca(int u, int v) {
    if(deep[u] < deep[v]) swap(u, v);
    int tmp = deep[u] - deep[v];
    REP(i, 20) if(tmp & (1 << i)) u = fa[u][i];
    if(u == v) return u;
    DWN(i, 19, 0) if(fa[u][i] != fa[v][i])
        u = fa[u][i], v = fa[v][i];
    return fa[u][0];
}

map<int, int>M;
void add(int x, int &ans) {
    visit[x] = 1;
    if(M.size() == 0) {
        M[dfn[x]] = x;
        return;
    }
    map<int, int>::iterator it1, it2;
    it1 = M.lower_bound(dfn[x]), it2 = it1;
    if(it1 == M.begin() || it1 == M.end()) it1 = M.begin(), it2 = M.end();
    it2--;

    int u = it1->second, v = it2->second;
    ans += dis[x] - dis[lca(u, x)] - dis[lca(v, x)] + dis[lca(u, v)];
    M[dfn[x]] = x;
}

void del(int x, int &ans) {
    visit[x] = 0;
    M.erase(dfn[x]);
    if(M.size() == 0) return;

    map<int, int>::iterator it1, it2;
    it1 = M.lower_bound(dfn[x]), it2 = it1;
    if(it1 == M.begin() || it1 == M.end()) it1 = M.begin(), it2 = M.end();
    it2--;

    int u = it1->second, v = it2->second;
    ans -= dis[x] - dis[lca(u, x)] - dis[lca(v, x)] + dis[lca(u, v)];
}

int main()
{
    int n, m, casnum, u, v, c, t, casid = 0;
    cin >> casnum;
    while(casnum--) {
        memset(visit, 0, sizeof(visit));
        cin >> n >> m;
        FOR(i, 1, n) head[i].clear();
        REP(i, n-1) {
            scanf("%d%d%d", &u, &v, &c);
            head[u].pb(pii(v, c));
            head[v].pb(pii(u, c));
        }
        t = 0;
        dfs(1, 0, t);
        int ans = 0;
        printf("Case #%d:\n", ++casid);
        M.clear();
        while(m--) {
            scanf("%d%d", &u, &v);
            if(u == 1 && !visit[v]) add(v, ans);
            if(u == 2 && visit[v]) del(v, ans);
            printf("%d\n", ans);
        }
    }
    return 0;
}

```
