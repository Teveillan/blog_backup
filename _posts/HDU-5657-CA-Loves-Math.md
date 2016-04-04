title: "HDU 5657 CA Loves Math"
date: 2016-04-03 20:44:47
categories: 红名之路
tags: [HDU]
description: 
---
传送门: http://acm.hdu.edu.cn/showproblem.php?pid=5657
题意：求[1, $A^n_{(10)}$]中，长度小于等于n，且为k的倍数的数有多少。

此题...巨坑...

比赛时思路是对的，对k进行分类讨论..然并卵...

等A了才发现几个大坑:
1. k的阀值在30000～60000之间才能A，高了低了都不行（想起了那些年计算几何中调精度的日子..）
2. queue真心慢...
3. n = 0 or n = 1时坑比较多，建议单独处理
4. **前导0是不算的!!!**。但是题目给的数据比较坑，刚好两位，如果算了前导0，刚好也对。（包括第一题中MOD值为1e8+7...我充满了对这个世界的怨念...）

所以说一直WA的可以按我上面的几点检查一下...我感觉我应该是把坑都踩完了...
<!--more-->
--------------------------正文------------------------------

对k进行分类，假设阀值为N（参考上面第一点）。
当k >= N时，枚举k的倍数，再判断数字是否重复。
当k < N时：
记dp[pos][j]：当用的数字集合为pos(状态压缩), 且mod k = j时的数目

转移方程：
$$ dp[ pos| (1<<i) ] [ (j + A^x ) \% k] += dp[pos][j] $$

```c++
#include <iostream>
#include <cstdlib>
#include <queue>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>
using namespace std;

#define REP(i,n) for(int i=0;i<(n);++i)
#define FOR(i,l,h) for(int i=(l);i<=(h);++i)
#define DWN(i,h,l) for(int i=(h);i>=(l);--i)

typedef long long LL;

const int N = 50000;

bool check(int A, LL x) {//判断数字是否重复
    int pos = 0;
    while(x) {
        int tmp = x % A;
        if(pos & (1<<tmp)) return 0;
        pos |= (1<<tmp);
        x /= A;
    }
    return 1;
}

int dp[1<<11][N], f[12];
LL solve(int A, int n, int k) {
    if(n == 0) return k == 1;
    if(n == 1) return A / k;

    n = min(n, A);
    LL NN = 1;
    REP(i, n) NN *= A;
    LL ret = 0;
    if(k >= N) { //k>=N时枚举k的倍数
        LL tmp = k;
        while(tmp <= NN) {
            if(check(A, tmp)) ++ret;
            tmp += k;
        }
        return ret;
    }

    f[0] = 1;
    FOR(i, 1, n) f[i] = (f[i-1] * A) % k;

    memset(dp, 0, sizeof(dp));
    dp[0][0] = 1;

    REP(pos, 1<<A) {
        int num = __builtin_popcount(pos);
        if(num >= n) continue;
        REP(i, A) if((pos & (1<<i)) == 0)
            REP(j, k) if(dp[pos][j]) {
                int x = pos | (1<<i), y = (j + i * f[num]) % k;
                dp[x][y] += dp[pos][j];
                if(i != 0 && y == 0) ret += dp[pos][j];
            }
    }
    if(check(A, NN) && (NN % k == 0)) ++ret; //特判A^n
    return ret;
}

int main()
{
    int casnum, A, n, k;
    cin >> casnum;
    while(casnum--) {
        cin >> A >> n >> k;
        cout << solve(A, n, k) << endl;
    }
    return 0;
}
```


