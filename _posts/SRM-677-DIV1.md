title: "SRM 677 DIV1"
date: 2016-04-06 12:58:13
categories: 红名之路
tags: [SRM]
description: 
---

##Easy:  DoubleOrOneEasy

给出a, b, A, B,有两种操作：
* a, b同时乘2
* a, b同时加1

问最少经过多少次操作使得 a == A && b == b
<!--more-->
首先明确一点，在什么情况下有解。

假如进行过x次乘法之后，a变为aa, b变为bb。
令$ aa = a \* 2^x + y, bb = b \* 2^x + z $，则y == z。

也就是说，如果有解，则存在一个x，使得
 $A - a \* 2^x == B - b \* 2 ^ x$

一般来说这样的x是只有一个的(A == B && a == b时特殊)。
贪心找到最大的x，就是最后进行过乘法的次数。
然后在此基础上找加法的个数。

还是令 $y = A - a * 2^x$
首先贪心，最开始先进行$y / (1<<x)$次加法操作, 还剩下$z = y \% (1<<x) $需要补，再加上bitcount(z)就好了（自己想为什么）。

```c++
int solve(int a, int b, int aa, int bb) {
    int ret = -1, num = 0;
    while(a <= aa && b <= bb) {
        if(aa - a == bb - b) ret = max(ret, num);
        a <<= 1, b <<= 1;
        ++num;
    }
    return ret;
}

class DoubleOrOneEasy
{
        public:
        int minimalSteps(int a, int b, int aa, int bb) {
            int num = solve(a, b, aa, bb);
            if(num == -1) return -1;
            int tmp = aa - (a << num);
            return num + tmp / (1<<num) + __builtin_popcount(tmp % (1<<num));
        }
};
```


##Medium: DiameterOfRandomTree
给出一个棵树,每条边等概率长度为1或2，求树的直径。

用`dp[root][d1][l1]`表示根为root的子树，直径为d1，从根往下最长的链长度为l1的方案数。
`dp[son][d2][l2]`表示子树的状态。
状态转移公式:

>$$ dp[root][max(max(d1, d2), l1 + l2 + 1)][max(l1, l2 + 1)] += dp[root][d1][l1] * dp[son][d2][l2] $$


>$$ dp[root][max(max(d1, d2), l1 + l2 + 2)][max(l1, l2 + 2)] += dp[root][d1][l1] * dp[son][d2][l2] $$

```c++
VI head[55];
LL dp[55][2][101][101], max_l[55], dia[55];//dp中加了一维作滚动数组
void init() {
    mem(dp), mem(max_l), mem(dia);
    REP(i, 50) head[i].clear();
}
int dfs(int cur, int pre) {
    int sta = 0;
    dp[cur][sta][0][0] = 1;
    for(int to: head[cur]) {
        if(to == pre) continue;
        int to_sta = dfs(to, cur); sta ^= 1;
        mem(dp[cur][sta]);
        FOR(d1, 0, dia[cur])
            FOR(l1, 0, max_l[cur])
                FOR(d2, 0, dia[to])
                    FOR(l2, 0, max_l[to]) {
                        int d = max(max(d1, d2), l1 + l2 + 1), l = max(l1, l2 + 1);
                        dp[cur][sta][d][l] += dp[cur][sta^1][d1][l1] * dp[to][to_sta][d2][l2];

                        d = max(max(d1, d2), l1 + l2 + 2), l = max(l1, l2 + 2);
                        dp[cur][sta][d][l] += dp[cur][sta^1][d1][l1] * dp[to][to_sta][d2][l2];
                    }
        dia[cur] = max(max(dia[cur], dia[to]), max_l[cur] + max_l[to] + 2);
        max_l[cur] = max(max_l[cur], max_l[to] + 2);
    }
    return sta;
}
class DiameterOfRandomTree
{
        public:
        double getExpectation(vector <int> a, vector <int> b){
            init();
            REP(i, a.size()) {
                head[a[i]].pb(b[i]);
                head[b[i]].pb(a[i]);
            }
            int sta = dfs(0, -1);
            double ret = 0, div = (double)(1ll << a.size());
            FOR(i, 0, dia[0])
                FOR(j, 0, max_l[0])
                    ret += (double) dp[0][sta][i][j] / div * i;
            return ret;
        }
};
```


