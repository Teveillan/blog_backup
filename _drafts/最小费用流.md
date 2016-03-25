title: 最小费用流
categories: Algorithm
date: 2015-01-04 15:21:44
tags:
description:
---
```c++
#define INF 1e9
#define N 1000
#define M 1000100
struct Edge
{
    int to,next,cost,flow;
}edge[M];
int head[N],ip;
int que[N],dis[N],pre[N];
bool visit[N];
bool spfa(int start,int end,int numpoint)
{
    int front=-1,tail=-1,to,top,temp;
    memset(visit,0,sizeof(visit));
    memset(pre,-1,sizeof(pre));
    for(int i=0;i<=numpoint;i++)    dis[i]=INF;
    que[++tail]=start; dis[start]=0;
    visit[start]=1;
    while(front!=tail)
    {
        if(++front>numpoint) front-=numpoint;
        top=que[front];
        visit[top]=0;
        for(int p=head[top];p!=-1;p=edge[p].next)
        {
            if(edge[p].flow>0)
            {
                to=edge[p].to;
                temp=edge[p].cost+dis[top];
                if(dis[to]>temp)
                {
                    dis[to]=temp;
                    pre[to]=p;                      //记录前驱边
                    if(!visit[to])
                    {
                        visit[to]=1;
                        if(++tail>numpoint) tail-=numpoint;
                        que[tail]=to;
                    }
                }
            }
        }
    }
    if(dis[end]==INF)   return 0;
    return 1;
}
int mincost(int start,int end,int numpoint)
{
    int u,mn;
    int ans_flow=0,ans_cost=0;
    while(spfa(start,end,numpoint))
    {
        u=end;
        mn=INF;
        while(pre[u]!=-1)
        {
            mn=min(edge[pre[u]].flow,mn);
            u=edge[ pre[u]^1 ].to;
        }
        u=end;
        while(pre[u]!=-1)
        {
            edge[pre[u]].flow-=mn;
            edge[pre[u]^1].flow+=mn;
            u=edge[ pre[u]^1 ].to ;
        }
        ans_cost+=dis[end]*mn;
       // ans_flow+=mn;
    }
    return ans_cost;
}
void add(int u,int v,int f,int c)
{
    edge[ip].to=v;edge[ip].flow=f;edge[ip].next=head[u];edge[ip].cost=c;head[u]=ip++;
    edge[ip].to=u;edge[ip].flow=0;edge[ip].next=head[v];edge[ip].cost=-c;head[v]=ip++;
}
```
