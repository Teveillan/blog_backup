title: "HDU 5291 Candy Distribution"
date: 2015-11-22 19:11:35
categories: 红名之路
tags: [HDU]
description: 
---

题目链接：http://acm.hdu.edu.cn/showproblem.php?pid=5291

题目大概意思就是有K种物品，第i种物品有ai个，要把全部的物品分成两堆，每个物品可以用也可以不用， 问最后两边物品数量一样多的方案数。

最直接的思路，定义dp[i][j]表示第i堆分完后第一个人比第二个人多j个糖果的方案数。枚举分第i堆糖果时第一个人比第二个人多分x个，则
> $$dp[i][j+x] += dp[i-1][j] * ( (a[i] - x) / 2 + 1 ) $$

<!--more-->

直接这么做的复杂度为$O(n^4)$，会超时。我们进行一次优化。

随着x的变化，$(a[i] - x) / 2 + 1$这个值根据奇偶性划分可分成两个等差数列。根据这个来进行处理，最后复杂度为O(n^3)。

```c++
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>
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
typedef vector<string> VS;
typedef vector<double> VD;
typedef long long LL;
typedef pair<int,int> pii;

const int N = 40010;
LL dp[2][N];
LL sum[N][2];
const LL MOD = 1e9 + 7;

inline LL get(int id, int flag, int mid) {
    if(id < 0) return 0;
    return sum[min(id, mid<<1)][flag];
}

int main()
{
    int casnum, n;
    cin >> casnum;
    while(casnum--) {
        memset(dp, 0, sizeof(dp));
        memset(sum, 0, sizeof(sum));
        cin >> n;
        int mid = n * 100;
        int cur = 0, pre = 1;
        dp[cur][mid] = 1;
        while(n--) {
            LL x;
            cin >> x;
            cur ^= 1, pre ^= 1;
            memset(dp[cur], 0, sizeof(dp[cur]));

            sum[0][0] = dp[pre][0];
            sum[0][1] = 0;
            FOR(i, 1, mid << 1) {
                bool flag = i & 1;
                sum[i][flag] = (sum[i-1][flag] + dp[pre][i]) % MOD;
                sum[i][flag^1] = sum[i-1][flag^1];
            }

            LL res = 0;
            FOR(i, 0, x) res = (res + (x - i + 2) / 2 * dp[pre][i]) % MOD;

            if(x & 1) {
                FOR(i, 0, mid << 1) {
                    bool flag = i & 1;
                    dp[cur][i] = res;
                    res = (res + get(i + x + 1, flag, mid) - get(i, flag, mid)) % MOD;
                    res = (res - (get(i, flag^1, mid) - get(i - x - 1, flag^1, mid))) % MOD;
                    res = (res + MOD) % MOD;
                }
            } else {
                FOR(i, 0 , mid << 1) {
                    bool flag = i & 1;
                    dp[cur][i] = res;
                    res = (res + get(i + x + 1, flag^1, mid) - get(i, flag^1, mid)) % MOD;
                    res = (res - (get(i, flag, mid) - get(i - x - 1, flag, mid))) % MOD;
                    res = (res + MOD) % MOD;
                }
            }
        }
        cout << dp[cur][mid] << endl;
    }
    return 0;
}

```
