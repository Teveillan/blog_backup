title: 最短路 SPFA
categories: Algorithm
date: 2015-01-04 15:32:34
tags:
description:
---
首先，为了简化分析，我们假定图中是没有负环的。
如果有负环，则就没有最短路径。
> 负环：一条从a到a的路径，路径上每条边的边权和为负。

但是SPFA算法是可以判断是否有负环存在的，比较基本的做法是判断每个点进队出队的次数，这个复杂度比较高。其实有一种DFS版的SPFA，用这个来判断是否有负环的效率是非常高的，这个我们后续再讲。

最短路的算法中都用着一种通用的思路：
* 初始化
* 松弛操作
<!-- more -->
初始化自然不用说。
最开始还没有进行任何操作，每个点（除了起始点）距离起始点的距离为无穷大（记为dis[i]=INF），起始点到起始点的距离为0（记作dis[start]=0）。


然后是松弛操作，也是本篇文章的主要部分。
松弛操作的要达到的目的是：
* ** 对于任意点u, 将从起始点到u的最短距离记为dis[u]。 **
* ** 对于点u的任意相邻点v, 满足dis[v] <= dis[u] + d[u,v] **

> u与v相邻：存在一条边 u->v

为什么要满足这样一个条件呢？

拿v点举例。
我们知道，一条从起始点s到v点的最短路径不可能是凭空产生的，从起始点到v点的最短路径在两种情况下可能产生：
* ** s与v相邻，(s,v)这条边就可能（但不是一定）就直接是一条最短路径。**
* ** 有一个点u与v相邻，通过s到u的的最短路径，延伸出一条从s到v得最短路径。 **

对于第一种情况，很明显一次就直接能找到最短路径。

对于第二种情况，假设到点v的最终最短路径是先从s到u, 再从u到v。 
在算法执行的过程中，我们不知道此时的dis[u]是否是最短距离。这个都没关系，我们先满足在当前dis[u]的情况下,dis[v]是否小于等于dis[u] + d[u,v]。
如果此时的dis[u]是最优解，dis[v]自然也是最优解。
如果dis[u]不是最优解，那就往前推，先保证从s到u的最短路径中，u的前一个点x是最优解。如果x不是，再往前推，早晚会推到dis[s]=0，这个妥妥地是最优解了，然后就能保证后面的都是最优解。

所以，我们的算法流程是这样:
1. 初始化
2. 将起始点加入一个队列中
3. 从队列中取出来一个点u，并将这个点从队列中移除
4. 对于u所有相邻的点v，看dis[v] 是否小于等于dis[u] + d[u,v]，如果不是（说明这个点还不是最优解，所以它之前更新过的点中仍然有些不是最优解），更新dis[v]=dis[u] + d[u,v]。再看v是否在队列中，如果不是，将v加入队列。
5. 回到步骤3
6. 直到队列为空，算法结束，此时的dis均为最优解。

算法复杂度为O(k * e)，e为边数。k为可变常数，可证明一般情况下k < 2。（我不知道怎么证明的...有兴趣的可以自己查查）

在这个模板中，为了省内存，我的队列采用的时循环队列的形式。因为每个点最多只能同时在队列里一次，所以这个队列大小为n就足够了。

```c++
class SPFA {
    typedef int type;
    struct Edge {
        int to, next;
        type d;
    }edge[M];

    type dis[N];
    int que[N], head[N], ip;
    bool visit[N];//标记各点是否在队列中

    public:
        SPFA() { init(); }

        void init() { //初始化，采用链式前向星建边
            memset(head, -1, sizeof(head));
            ip = 0;
        }

        void add(int u, int v, int d) {
            edge[ip].to=v; edge[ip].d=d; edge[ip].next=head[u]; head[u]=ip++;
        }

        void spfa(int start, int end, int num) {
            memset(visit, 0, sizeof(visit));//清空队列
            for(int i=0; i<=num; ++i)   dis[i] = INF;//步骤1

            int front = -1, tail = -1;
            dis[start] = 0; visit[start] = 1; que[++tail] = start;//步骤2
            while(front != tail) {
                if(++front > num)   front -= num;
                int top = que[front];//步骤3
                visit[top] = 0;
                for(int p=head[top]; p!=-1; p=edge[p].next) {
                    int to = edge[p].to, temp = dis[top] + edge[p].c;
                    if(dis[to] > temp) {//步骤4
                        dis[to] = temp;
                        if(!visit[to]) {
                            if(++tail > num)    tail -= num;
                            que[tail] = to;
                            viist[to] = 1;
                        }
                    }
                }
            }
            return dis[end];
        }
}
```
