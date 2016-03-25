title: CDQ分治 && HDU 5126
categories: Algorithm
date: 2015-05-02 16:48:26
tags:
    - HDU
    
description:
---
参考文献：[从《Cash》谈一类分治算法的应用](http://wenku.baidu.com/view/52f9c11cff00bed5b9f31d2d.html) -- 陈丹琦

也因此简称CDQ分治。

个人理解CDQ分治和普通分治最大的区别为：
普通分治可以将问题拆成几个相互独立的子问题
CDQ分治将问题分成了两个部分：
1.  相互独立的两个子问题  
2.  两个子问题之间的联系

因此，CDQ分治的算法流程也分为以下几步：
> 1.  算法开始
> 2.  取一个中间点mid, 将问题分为[L, mid] 和 [mid + 1, R]两个部分。分别处理两个子问题
> 3.  处理[L, mid] 和 [mid + 1, R]之间的联系

<!--more-->

举个例子：在二维坐标系中，有n个点，m个询问。对于每个询问(x, y)，求点(x1, x2) ，x1<= x && y1 <= y 有多少个。

当然，这题用树状数组就直接能写了。但是我们在这里用CDQ分治来解决一下这个问题。

我们将n个点看做是n个更新，并和m个询问放在一起。定义一个结构体用vector存起来。

```c++
void sovle1(int l, int r, vector<Struct>vec1) {
    //在这里将vec1[l, r]按x从小到大排序
    int mid = (l + r) / 2;
    solve1(l, mid);
    solve1(mid + 1, r);//划分成相互独立的子问题

    //将vec1[l, mid]中的更新操作 和 vec1[mid + 1, r]中的询问操作,存到一个vec2中 
    solve2(l, r, vec2);//左半边的更新操作和右半边的询问操作之间有联系
}

void solve2(int l, int r, vector<Struct>vec2) {
//此时在solve2中已经不需要管x了。在vec2中所有的更新操作的x必然小于等于询问操作的x。
    
    //在这里将vec2[l, r]按y从小到大排序
    int mid = (l + r) / 2;
    sovle2(l, mid);
    solve2(mid + 1, r);//独立子问题

    //将vec2[l, mid]中的更新操作和vec2[mid + 1, r]中得询问操作存到一个vec3中
    solve3(l, r, vec3);//
}

void solve3（int l, int r, vector<Struct>vec3） {
//此时在solve3中，x和y都是已经排好序的。从前到后遍历该更新更新该询问询问就好。
    int cal = 0;
    for(Struct tmp: vec3) {
        if(tmp.type == Update)  ++cal;
        else tmp.query_anser += cal;
    }
}
//这段代码其中有许多可以优化的地方，但是不要在意细节，主要是为了更加方便地理解算法
```

虽然解法不如树状数组，但是我们得看到题目扩展后的好处。
我们看到，当到solve2时，函数中的参数`vec2`已经不用再管他的x了（`请务必深刻理解为什么不用再管了`）。
这样的好处在哪里？没错，**降维！**
假如我们要处理的不是二维，而是三维四维依旧可以这么处理，每一个维度只不过加了O(log)的复杂度而已。
**如果是随时可以更新查询的操作，只不过也只是把时间当成一个维度而已。**（光是这一点就已经牛逼哄哄够解决大部分题目了。）

然后就是下面这道题，三维且可随时更新查询：
HDU 5126 starts: [传送门](http://acm.hdu.edu.cn/showproblem.php?pid=5126)

```c++
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <vector>
#include <cstdlib>

using namespace std;

#define FOR(i,l,r)  for(int i=(l); i<=(r); ++i)
#define REP(i,r)    for(int i=0; i<(r); ++i)
#define DWN(i,r,l)  for(int i=(r);i>=(l);--i)

#define pb push_back

typedef long long ll;
typedef pair<int, int>pii;

const int N = 5e4 + 100;
int que[N << 1];
int ans[N];
int z_num;
int node[N << 1];
struct Query {
    int x, y, z, kind, id;
    Query() {}
    Query(int _x, int _y, int _z, int _kind, int _id) : x(_x), y(_y), z(_z), kind(_kind), id(_id) {}
}queries[N << 3], queries_x[N<<3], queries_y[N << 3];

bool cmpx(Query a, Query b) {
    return a.x < b.x || (a.x == b.x && a.kind < b.kind);
}
bool cmpy(Query a, Query b) {
    return a.y < b.y || (a.y == b.y && a.kind < b.kind);
}

inline int lowbit(int x)    {return x&-x;}
void update(int pos, int value) {
    while(pos <= z_num) {
        node[pos] += value;
        pos += lowbit(pos);
    }
}
int query(int pos) {
    int ret = 0;
    while(pos) {
        ret += node[pos];
        pos -= lowbit(pos);
    }
    return ret;
}

void solve3(int l, int r) {//只剩下二维y和z，直接就是二维的更新查询题目，树状数组解决就好了
    if(l >= r)  return;
    FOR(i, l, r)
        if(queries_y[i].kind == 0)  update(queries_y[i].z, 1);
        else if(queries_y[i].kind == 1)  ans[ queries_y[i].id ] -= query(queries_y[i].z);
        else if(queries_y[i].kind == 2) ans[ queries_y[i].id ] += query(queries_y[i].z);
    FOR(i, l, r)
        if(queries_y[i].kind == 0)  update(queries_y[i].z, -1);
}


void solve2(int l, int r) {//把x这一维度消除
    if(l >= r)  return;
    int mid = (l+r)>>1;
    solve2(l, mid);
    solve2(mid + 1, r);

    int num = 0;
    FOR(i, l, mid)  if(queries_x[i].kind == 0)  queries_y[num++] = queries_x[i];
    FOR(i, mid+1, r)    if(queries_x[i].kind)  queries_y[num++] = queries_x[i];
    sort(queries_y, queries_y + num, cmpy);
    solve3(0, num - 1);
}

void solve1(int l, int r) {//已经默认时间从小到大排序了，把时间这一维度消除
    if(l >= r)  return;
    int mid = (l+r)>>1;
    solve1(l, mid);
    solve1(mid + 1, r);
    int num = 0;
    FOR(i, l, mid)  if(queries[i].kind == 0)    queries_x[num++] = queries[i];
    FOR(i, mid + 1, r)  if(queries[i].kind)     queries_x[num++] = queries[i];
    sort(queries_x, queries_x + num, cmpx);//对x从小到大排序
    solve2(0, num - 1);
}


int main() {
    int casnum, n;
    int x1, x2, y1, y2, z1, z2, kind;
    cin >> casnum;
    while(casnum--) {
        memset(ans, -1, sizeof(ans));
        cin >> n;
        int id = 0;
        z_num = 0;
        REP(i, n) {
            scanf("%d", &kind);
            if(kind == 1) {
                scanf("%d%d%d", &x1, &y1, &z1);
                queries[id++] = Query(x1, y1, z1, 0, i);
                que[z_num++] = z1;
            } else {
                ans[i] = 0;
                scanf("%d%d%d%d%d%d", &x1, &y1, &z1, &x2, &y2, &z2);
                queries[id++] = Query(x2    , y2    , z2    , 2, i);
                queries[id++] = Query(x2    , y2    , z1 - 1, 1, i);
                queries[id++] = Query(x2    , y1 - 1, z2    , 1, i);
                queries[id++] = Query(x1 - 1, y2    , z2    , 1, i);
                queries[id++] = Query(x2    , y1 - 1, z1 - 1, 2, i);
                queries[id++] = Query(x1 - 1, y2    , z1 - 1, 2, i);
                queries[id++] = Query(x1 - 1, y1 - 1, z2    , 2, i);
                queries[id++] = Query(x1 - 1, y1 - 1, z1 - 1, 1, i);
                que[z_num++] = z2;
                que[z_num++] = z1 - 1;
            }
        }
            sort(que, que + z_num);
            z_num = unique(que, que + z_num) - que;
            REP(i, id)  queries[i].z = lower_bound(que, que + z_num, queries[i].z) - que + 1;
            solve1(0, id - 1);
            REP(i, n)
                if(ans[i] != -1)
                    printf("%d\n", ans[i]);
    }

    return 0;
}
```
