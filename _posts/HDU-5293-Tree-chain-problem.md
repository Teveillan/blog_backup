title: "HDU 5293 Tree chain problem"
date: 2015-11-23 23:39:29
categories: 红名之路
tags: [HDU]
description: 
---


题目链接: http://acm.hdu.edu.cn/showproblem.php?pid=5293

题目大意：有个N的点的树。其中树上有M条已知的链，每条链有一个权值。从中选出任意个不相交的链使得链的权值和最大。

树形DP。
我们用dp[i]表示以i点为根的子树的最优解。
用sum[i]表示以i点为根的子树中不选i点的最优解，即$sum[i]=\sum dp[k] $  (k为i的儿子节点)

<!--more-->

其中dp[i]不一定是一定要选取i点,因此，dp[i]也有可能等于sum[i]。

我们假设出一种情况，一颗以root为根的树，有一条链link: root -> a -> b ， 权值为x。
在知道除了root之外其他所有点的dp值及sum值已知的情况下，求出选取这条链情况下的root的dp值。

首先，sum[root]是可以求的。那么
> $$ dp[root] = sum[root] - dp[a] + sum[a] - dp[b]+ sum[b] + x $$

(联系sum的定义理解这个式子)

理解以后这道题就迎刃而解了，无非就是求一次lca，然后对于如何快速在树上求一条链的sum和以及dp和做一次优化而已，用时间戳或者树链剖分的方法都好。

我这里用的时间戳加树状数组来进行优化的。

```c++
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <iostream>
#include <stack>
using namespace std;

#define pb push_back

#define REP(i,n) for(int i=0;i<(n);++i)
#define FOR(i,l,h) for(int i=(l);i<=(h);++i)
#define CLR(arr) memset(arr, 0, sizeof(arr))

typedef vector<int> VI;
typedef pair<int,int> pii;

const int N = 2e5 + 100;
int node[2][N];
inline int lowbit(int x) {return x&-x;}
void update(int x, int value, int n, int flag) {
    while(x <= n) {
        node[flag][x] += value;
        x += lowbit(x);
    }
}
int query(int x, int flag) {
    int ret = 0;
    while(x) {
        ret += node[flag][x];
        x -= lowbit(x);
    }
    return ret;
}

VI head[N], vec[N];
vector<pii> lca[N], qhead[N];
int fa[N];
int find(int x) {
    if(x == fa[x]) return x;
    else return fa[x] = find(fa[x]);
}

bool visit[N];
void LCA(int cur, int pre) {
    visit[cur] = 1;
    REP(i, head[cur].size()) {
        int to = head[cur][i];
        if(visit[to]) continue;
        LCA(to, cur);
    }
    REP(i, qhead[cur].size()) {
        int to = qhead[cur][i].first, x = qhead[cur][i].second;
        if(visit[to]) {
            int tmp = find(to);
            lca[tmp].pb(pii(cur, to));
            vec[tmp].pb(x);
        }
    }
    fa[cur] = pre;
}

int l[N], r[N];
int dp[N], sum[N];
int t;
void dfs(int cur, int n) {
    visit[cur] = 1;
    l[cur] = ++t;
    REP(i, head[cur].size()) {
         int to = head[cur][i];
         if(visit[to]) continue;
         dfs(to, n);
         sum[cur] += dp[to];
    }
    REP(i, lca[cur].size()) {
        int u = lca[cur][i].first, v = lca[cur][i].second, x = vec[cur][i];
        int tmp = sum[cur] + query(l[u], 0) + query(l[v], 0) - query(l[u], 1) - query(l[v], 1) + x;
        dp[cur] = max(tmp, dp[cur]);
    }
    r[cur] = ++t;
    dp[cur] = max(dp[cur], sum[cur]);
    update(l[cur], sum[cur], n*2, 0);
    update(r[cur], -sum[cur], n*2, 0);
    update(l[cur], dp[cur], n*2, 1);
    update(r[cur], -dp[cur], n*2, 1);
}

int main()
{
    int casnum, n, m, u, v, x;
    cin >> casnum;
    while(casnum--) {
         cin >> n >> m;
         CLR(node);
         FOR(i, 1, n) head[i].clear(), qhead[i].clear(), lca[i].clear(), vec[i].clear();
         FOR(i, 1, n) fa[i] = i;
         REP(i, n-1) {
            scanf("%d%d", &u, &v);
            head[u].pb(v);
            head[v].pb(u);
         }

         REP(i, m) {
             scanf("%d%d%d", &u, &v, &x);
             qhead[u].pb(pii(v, x));
             qhead[v].pb(pii(u, x));
         }
         memset(sum, 0, sizeof(sum));
         memset(dp, 0, sizeof(dp));
         t = 0;
         CLR(visit);
         LCA(1, 1);
         CLR(visit);
         dfs(1, n);
         cout << dp[1] << endl;
    }
    return 0;
}
```
