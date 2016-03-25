title: "HDU 5208 Where is Bob && Bestcoder Round38 C题"
categories: 红名之路
tags:
  - BestCoder
date: 2015-04-21 17:57:45
description:
---

题目链接：http://acm.hdu.edu.cn/showproblem.php?pid=5208

近似于数位DP的思路。
用数位DP求区间中符号条件的数有多少的时候，有时会用一个符号位来记录当前的取值是否达到区间的上界。这里同样用这个办法。（官方题解我有点没看懂，我的解法有可能和官方题解不一样）

看到题第一眼，觉得应该优先从高位到低位处理，在以下两种情况中，决策应该是确定的：
*   第一个人只能取0或1中的一个。这时候第二个人一定是尽量取和第一个人一样的值。
*   第二个人只能取0或1中得一个，而第一个人两个都可以选。第一个人自然取第二个人取不到的值。

但是，当在这两种情况之外，决策就不唯一了。这时，先把低位的决策解决了就很有必要。
<!-- more -->
我们用dp[i][l1][r1][l2][r2]来表示。 
> `l1` 为1时，表示 **假设第一个人从最高位到第i+1位的取值都是取区间范围允许的最小值。** 此时，第一个人在第i位的取值要小心不要小于区间允许的最小值。
反之`l1`为0时，表示**假设第一个人从最高位到第i+1位的取值有一个不是区间范围的最小值**，这时候，从第i位到第0位取任何值都不会小于区间允许的最小值了。

请仔细理解上面这段话。

同理，`r1`代表之前取的是不是区间范围允许的最大值。然后能类推倒l2, r2。

对于每一位, 我们枚举l1, r1, l2, r2。**（再次阐明一下，l1, r1, l2, r2不是表示第i位两个人的取值范围，是表示高位的取值是不是都是边界值。）** 当l1, r1, l2, r2确定以后，第i位两个人的取值范围也就确定了，枚举两个人的取值，用第i-1位递推到第i位。

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

ll dp[40][2][2][2][2];

int main() {
    int casnum, casid = 0;
    cin >> casnum;
    while(casnum--) {
        REP(i, 31)
            REP(a, 2)
                REP(b, 2)
                    REP(c, 2)
                        REP(d, 2)
                            dp[i][a][b][c][d] = 1e15;
        int l[2][2];
        REP(i, 2)
            REP(j, 2)
                cin >> l[i][j];

        FOR(i, 0, 31) {
            REP(a, 2)
                REP(b, 2)
                    REP(c, 2)
                        REP(d, 2) { //枚举高位的取值是不是都是边界值
                            bool tmp_l[2][2] = { {0, 1}, {0, 1} }; 
                            // 确定此时两个人的取值范围
                            if(a)   tmp_l[0][0] = l[0][0] & (1<<i);
                            //当第一个人高位的取值都是下界值，而且第i位的下界值为1时
                            //第i位的取值范围的下界为1。否则第i位的取值范围的下界为0。
                            if(b)   tmp_l[0][1] = l[0][1] & (1<<i);
                            //都类比第一个
                            if(c)   tmp_l[1][0] = l[1][0] & (1<<i);
                            if(d)   tmp_l[1][1] = l[1][1] & (1<<i);

                            ll tmp1 = 0;
                            FOR(j, tmp_l[0][0], tmp_l[0][1]) {
                            //取值范围确定了，现在我们来枚举值。j是第一个人的取值
                                ll tmp2 = 1e15;
                                FOR(k, tmp_l[1][0], tmp_l[1][1]) {
                                    //第i位的异或值加上i-1位的dp值就是第i位的dp值。
                                    //但是我们得先算出两个人分别取j和k的情况下
                                    //高位的取值加上第i位的取值是否依然达到上下界
                                    bool tmp[2][2] = { {0, 0}, {0, 0} };
                                    if(a && (j == tmp_l[0][0])) tmp[0][0] = 1;
                                    //当且仅当高位取值到达下界,第i位又取到下界值得时候,
                                    //对i-1位来说高位的取值达到下界
                                    if(b && (j == tmp_l[0][1])) tmp[0][1] = 1;
                                    if(c && (k == tmp_l[1][0])) tmp[1][0] = 1;
                                    if(d && (k == tmp_l[1][1])) tmp[1][1] = 1;
                                    if(i)
                                        tmp2 = min(tmp2, ((ll)(j^k) << i) + dp[i-1][tmp[0][0]][tmp[0][1]][tmp[1][0]][tmp[1][1]]);
                                    else
                                        tmp2 = min(tmp2, ((ll)(j^k) << i));
                                }
                                tmp1 = max(tmp1, tmp2);
                            }
                            dp[i][a][b][c][d] = tmp1;
                        }
        }
        ll ans = dp[31][1][1][1][1];
        printf("Case #%d: %I64d\n", ++casid, ans);

    }
    return 0;
}
```
