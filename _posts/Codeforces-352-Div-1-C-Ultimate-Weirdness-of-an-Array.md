title: "Codeforces #352 Div.1 C. Ultimate Weirdness of an Array"
date: 2016-05-18 23:24:11
categories: 红名之路
tags: [Codeforces]
description: 
---

对数论不是很在行...每次碰到gcd相关的题都是一脸懵逼...以后遇到这种题还是多总结一下吧。

最初以为是要分解质因子，然后用容斥乱搞一番，结果发现完全搞不出来..

题意: $1 <= n <= 2 * 10^5$， 求
 $$ \sum_{1 <= l <= r <= n} max(gcd(a_i, a_j))  ~~~  {( i, j\notin [l, r])}$$

<!--more-->
参考思路： http://async.icpc-camp.org/d/418-codeforces-round-352e-ultimate-weirdness-of-an-array/4

我们维护一个集合S，集合中记录的是一些区间[l, r]。对于集合中任一一个区间[l, r]，使得$max(gcd(a_i, a_j)) < d~ (i, j, \notin [l, r])$时，我们记录此时的区间个数A[d] = S.size()。

可得：
$$ans = \sum d * num[d] ~~~(num[d] = A[d+1] - A[d])$$


然后需要注意的地方便是如何维护集合S:
最初S中有n * (n + 1) 个元素。
从大到小枚举d，此时我们想从集合S删掉的元素为：当删掉[l, r]后, max_gcd为d的倍数。

我们将所有d的倍数列出来，进行排序。此时需要删掉的区间有：
1. 删掉中间一部分，使得左右各至少有一个d的倍数。
2. 删掉左边的一部分，最少保留最右边的两个d的倍数。
3. 删掉右边的一部分，最少保留最左边的两个d的倍数。

需要删的部分确定了，接下来就是确定怎么删的问题。暴力会TLE，直接减也会导致重复。

对于最初所有的二元数组[l, r] (l <= r)， 我们可以看作为一个以 x=y 为底边，x轴为腰的一个等腰直角三角形。
每当删除一个区间[l, r]，对于任意区间[i, j] ($ l <= i <= j <= r$) ，我们也应该删除。
在图形上表示出来的意义就是，对于任意的i (l <= i <= r)，删掉 j <= r 的部分。（自己动手画一下）。于是便可以用数据结构进行维护了。

我用的线段树：

```c++
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>
using namespace std;

#define REP(i,n) for(int i=0;i<(n);++i)
#define FOR(i,l,h) for(int i=(l);i<=(h);++i)
#define DWN(i,h,l) for(int i=(h);i>=(l);--i)
#define MEM(arr, v) memset(arr, v, sizeof(arr))

const int N = 2e5 + 10;

#define L LL, l, MID
#define R RR, MID + 1, r
#define LL mask << 1
#define RR mask << 1 | 1
#define MID ((l + r) >> 1)

int mn[N << 2], lazy[N << 2], mx[N << 2];
long long area[N << 2];

void push_down(int mask, int l, int r) {
    if(lazy[mask] == -1) return;
    mn[LL] = mn[RR] = mx[LL] = mx[RR] = lazy[mask];
    area[LL] = (long long)(MID - l + 1) * mn[LL];
    area[RR] = (long long)(r - MID) * mn[RR];
    lazy[LL] = lazy[RR] = lazy[mask];
    lazy[mask] = -1;
}
void push_up(int mask) {
    mn[mask] = min(mn[LL], mn[RR]);
    mx[mask] = max(mx[LL], mx[RR]);
    area[mask] = area[LL] + area[RR];
}
void update(int mask, int l, int r, int ll, int rr, int h) {
    if(ll > r || rr < l || h <= mn[mask]) return;
    if(l == r) {
        mn[mask] = mx[mask] = area[mask] = max(mx[mask], h);
        return;
    }
    if(ll <= l && rr >= r && h >= mx[mask]) {
        mn[mask] = lazy[mask] = mx[mask] = h;
        area[mask] = (long long)(r - l + 1) * h;
    } else {
        push_down(mask, l, r);
        update(L, ll, rr, h);
        update(R, ll, rr, h);
        push_up(mask);
    }
}
long long ans[N];
int l[N], r[N], l2[N], r2[N];
int main() {
    int n, x;
    while(cin >> n) {
        MEM(mn, 0), MEM(mx, 0), MEM(area, 0), MEM(lazy, -1), MEM(l, -1), MEM(r, -1), MEM(l2, -1), MEM(r2, -1);
        int m = 0;
        FOR(i, 1, n) {
            scanf("%d", &x);
            m = max(x, m);
            for(int j = 1; j * j <= x; ++j) { //其实对于每个d，只用维护最左和最右两个数就够了
                if(x % j) continue;
                if(l[j] == -1) l[j] = i;
                else if(l2[j] == -1) l2[j] = i;
                r2[j] = r[j], r[j] = i;

                if(j * j == x) continue;

                if(l[x / j] == -1) l[x / j] = i;
                else if(l2[x / j] == -1) l2[x / j] = i;
                r2[x / j] = r[x / j], r[x / j] = i;
            }
        }

        FOR(i, 1, n) update(1, 1, n, i, i, i-1); // 最开始都是矩形，剪掉一部分成三角形的形状
        DWN(i, m + 1, 1) {
            if(l[i] != r[i]) {
                update(1, 1, n, l[i]+1 , r[i]-1  , r[i] - 1); //删掉中间
                update(1, 1, n, 1      , r2[i]- 1, r2[i] - 1);//删左边
                update(1, 1, n, l2[i]+1, n       , n);//删右边
            }
            ans[i] = (long long)n * n - area[1];//前面加不加n*n不影响，但是加上以后实际意义为剩余三角形的面积
        }
        long long ret = 0;
        FOR(i, 1, m) ret += (ans[i+1] - ans[i]) * i;
        cout << ret << endl;
    }
    return 0;
}
```


