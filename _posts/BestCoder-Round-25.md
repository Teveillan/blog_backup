title: "BestCoder Round #25"
categories: 红名之路
tags:
  - BestCoder
date: 2015-01-09 13:57:46
description:
---
## 1001 Harry and Magical Computer

就是判断有没有环存在。
点比较少，Floyd一遍O(n^3)就行。

## 1002 Harry And Magic Box

官方题解是O(n^4)的，其实中间有一维可以省略。（小地方而已。。不管也罢。。）
<!-- more -->
dp[i][j]表示第i行已经有j列被选过（这j个不一定全都是在第i行选的，有可能包括之前选的）。
则dp公式为： `(1 <= k <= j)`

```c++
if(k == j)
    dp[i][j] += dp[i-1][k] * ((1<<k) - 1);
else:
    dp[i][j] += dp[i-1][k] * (1<<k) * c[m-k][j-k];
```
表示假如`k==j`，前k个中至少选一个，种类数为`((1<<k) - 1)`。
否则， 前k个随便选， 其余从`m-k`中选`j-k`个。
注意数据溢出。

## 1003 Harry and Christmas tree

我是按照官方题解的思路。
对每种颜色单独考虑。对第i-1个有这种颜色点a, 第i个有这种颜色的点b， 以及a和b的lca点c。 在递归过程中，c之前的点都可以被贡献1中颜色，直到c以及c往上，贡献度为a和b各一个，然后减去a和b的重复一个，总共为1。

中间要用到lca算法，因为这道题就是按照递归的顺序进行遍历，用离线的lca就行。
```c++
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cstring>
#include <string>
#include <stack>
#include <queue>
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;

#define FOR(i,l,r)  for(int i=(l); i<=(r); ++i)
#define REP(i,r)    for(int i=0; i<(r); ++i)
#define DWN(i,r,l)  for(int i=(r);i>=(l);--i)

#define pb push_back

typedef long long ll;
typedef pair<int, int>pii;
const int N = 5e4 + 1;
const int M = 1e5 + 1;
vector<int>head[N], gift[N];
int fa[N], ans[N];
int pre_color[M];

inline int find(int x) {
    if(x==fa[x])    return x;
    else return fa[x] = find(fa[x]);
}

void dfs(int now, int pre) {
    REP(i, head[now].size()) {
        if(head[now][i] != pre) {
            dfs(head[now][i], now); 
            fa[ head[now][i] ] = now; //离线lca中得一部分
            ans[now] += ans[ head[now][i] ]; //将子节点的贡献度加上来。（这时候子节点的贡献度已经是 总贡献度 - 重复次数 了）
        }
    }
    REP(i, gift[now].size()) {
        int color = gift[now][i];
        if(!pre_color[color])  { //第一个出现这种颜色，跳过不处理
            pre_color[color] = now;
            continue;
        }
        int lca = find(pre_color[color]); // 找lca
        --ans[lca]; // 在lca上加上一个重复度, ans--

        pre_color[color] = now;
    }
    ans[now] += gift[now].size(); //加上本节点的贡献
}

int main() {
    //freopen("in", "r", stdin);
    int n, m;
    int x, y;
    while(cin>>n>>m) {
        memset(ans, 0, sizeof(ans));
        memset(pre_color, 0, sizeof(pre_color));
        FOR(i, 1, n)    fa[i] = i, head[i].clear(), gift[i].clear();
        REP(i, n-1) {
            scanf("%d%d",&x, &y);
            head[x].pb(y);
            head[y].pb(x);
        }
        while(m--) {
            scanf("%d%d", &x, &y);
            gift[x].pb(y);
        }
        dfs(1, 0);

        FOR(i, 1, n) {
            if(i!=1)    putchar(' ');
            printf("%d", ans[i]);
        }
        puts("");
    }
    return 0;
}
```

## Harry and magic string

DP + Manacher算法

DP思路很简单。
sum[i] 表示以i结尾和i之前结尾的回文串总数。
dp[i] 表示以i开头的回文串数。

> ans = $$\sum_{i=1}^{len-1} {sum[i-1] * dp[i]}$$

难点就是`sum[i]` 和 `dp[i]` 怎么求的问题了。
新学习了个算法， Manacher算法，在我转载的上篇文章中有讲解。[传送门](http://teveillan.com/2015/01/09/-%E8%BD%AC-Manacher%E7%AE%97%E6%B3%95-%E5%9B%9E%E6%96%87%E4%B8%B2%E5%A4%84%E7%90%86/)

```c++
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cstring>
#include <string>
#include <stack>
#include <queue>
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;

#define FOR(i,l,r)  for(int i=(l); i<=(r); ++i)
#define REP(i,r)    for(int i=0; i<(r); ++i)
#define DWN(i,r,l)  for(int i=(r);i>=(l);--i)

#define pb push_back

typedef long long ll;
typedef pair<int, int>pii;

const int MAX_LEN = 1e5 + 10;

char str[MAX_LEN];
int Len[MAX_LEN<<1];
struct Manacher { // Manacher算法, 讲解看上篇文章
    char tmp_str[MAX_LEN<<1];
    int init(char *s) {
        int len = strlen(s);
        int id = 0;
        tmp_str[id++] = '#';
        for(int i=0; i<len; ++i) {
            tmp_str[id++] = s[i];
            tmp_str[id++] = '#';
        }
        tmp_str[id] = '\0';
        return id;
    }

    void solve(char *s) {
        int len = init(s);
        int max_r = -1, max_id = -1;
        for(int i=0; i<len; ++i) {
            if(max_r > i)   Len[i] = min(max_r - i, Len[2 * max_id - i]);
            else Len[i] = 1;

            while(i - Len[i] >= 0 && i + Len[i] < len && tmp_str[i - Len[i]] == tmp_str[i + Len[i]])
                ++Len[i];

            if(i + Len[i] > max_r) {
                max_r = i + Len[i];
                max_id = i;
            }
        }
    }
};

long long sum[MAX_LEN << 1];
int flag[MAX_LEN << 1];
int main() {
    //freopen("in", "r", stdin);
    while(cin>>str) {
        memset(sum, 0, sizeof(sum));
        memset(flag, 0 ,sizeof(flag));
        Manacher manacher;
        manacher.solve(str);

        int len = strlen(str);
        int tmp = 0;
        for(int i = 1; i <= (len<<1); ++i) { //求sum
            sum[i] += sum[i-1];
            ++tmp;

            tmp -= flag[i];
            ++flag[i + Len[i]];
            if(i&1) sum[i] += tmp;
        }

        long long ans = 0;
        memset(flag, 0, sizeof(flag));
        tmp = 0;
        for(int i = (len<<1); i >= 1; --i) { //求ans
            ++tmp;
            tmp -= flag[i];
            ++flag[i - Len[i]];
            if(i&1) ans += sum[i-1] * tmp;
        }
        cout << ans << endl;
    }
    return 0;
}

```


