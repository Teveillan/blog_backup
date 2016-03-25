title: "HDU 5290 Bombing plan"
date: 2015-11-19 16:07:43
categories: 红名之路
tags: [HDU]
description: 
---

题目链接： http://acm.hdu.edu.cn/showproblem.php?pid=5290

官方题解已经说的很详细了。我也引用一下官方题解吧（有修改，加重部分）。

>  DP,定义两个数组$F[I][J]$,$G[I][J]$。
>  $F[I][J]$表示以第i个点为根的子树上的点被全部破坏掉，并且还能向上破坏掉**不少于**j个距离，满足这样条件需要选取的最小点数；
>  $G[I][J]$表示以第i个点为根的子树上的点被未被全部破坏，且未被破坏的点中距离i最远的点距离为**不超过**j，满足这样条件需要选取的最小点数。
<!--more-->

那么可以写出dp方程:
$$G[I][J]=min(G[L][0],G[L][1],...,G[L][J-1]) $$ 
L 代表所有儿子。

$$F[I][J] = F[K][J+1]+G[L][J]$$
$$F[I][J] = min(F[I][j], F[I][J+1])$$
式子中加号左边的意思是i节点必须存在一个儿子K，选取F[K][J+1]这个值。加号右边的式子表示除K以外其他儿子L的取值。

```c++
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;

#define pb push_back
#define mp make_pair

#define REP(i,n) for(int i=0;i<(n);++i)
#define FOR(i,l,h) for(int i=(l);i<=(h);++i)
#define DWN(i,h,l) for(int i=(h);i>=(l);--i)

typedef vector<int> VI;
typedef long long LL;

const int N = 1e5 + 100;
int dp[N][111], f[N][111];
int w[N];
VI head[N];

void dfs(int cur, int pre) {
    FOR(i, 0, 101) dp[cur][i] = 0, f[cur][i] = (i<=w[cur]?1:1e6); 
    //分别初始化取和不取时候的初始值。后面就没必要分步讨论了。

    for(int to: head[cur]) {
        if(to == pre) continue;
        dfs(to, cur);

        DWN(i, 100, 1) {
            f[cur][i] = min(f[cur][i] + dp[to][i-1], dp[cur][i] + f[to][i+1]);
            f[cur][i] = min(f[cur][i], f[cur][i+1]);
        }
        f[cur][0] = min(f[cur][0] + f[to][0], dp[cur][0] + f[to][1]);
        f[cur][0] = min(f[cur][0], f[cur][1]);

        dp[cur][0] = min(f[cur][0], dp[cur][0] + f[to][0]);
        FOR(i, 1, 100) {
            dp[cur][i] = min(dp[cur][i-1], dp[cur][i] + dp[to][i-1]);
        }
    }
}

int main() {
	//freopen("in", "r", stdin);

    int n;
    while(cin >> n) {
        FOR(i, 1, n) head[i].clear();
        FOR(i, 1, n) scanf("%d", &w[i]);

        REP(i, n-1) {
             int x, y;
             scanf("%d%d", &x, &y);
             head[x].pb(y);
             head[y].pb(x);
        }
        if(n == 1) {
             puts("1");
             continue;
        } else {
            dfs(1, 0);
            cout << f[1][0] << endl;
        }
    }
    return 0;
}
```
