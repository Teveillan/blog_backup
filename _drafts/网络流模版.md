title: "网络流模版"
tags: [Model]
---

//int型
#define N 100
#define M 100
#define INF 1e8
struct
{
    int to,next;
    int c;
}edge[M];
int head[N],level[N],ip;
int que[N];
bool makelevel(int s,int t)
{
    memset(level,0,sizeof(level));
    int iq=0;
    que[iq++]=s;
    level[s]=1;
    int top;
    for(int i=0;i<iq;i++)
    {
        top=que[i];
        if(top==t)  return 1;
        for(int k=head[top];k!=-1;k=edge[k].next)
        {
            if(!level[edge[k].to]&&edge[k].c>0)
            {
                que[iq++]=edge[k].to;
                level[edge[k].to]=level[top]+1;
            }
        }
    }
    return 0;
}
int dfs(int now,int maxf,int t)
{
    if(now==t)  return maxf;
    int  ret=0,c;
    for(int k=head[now];k!=-1;k=edge[k].next)
    {
        if(edge[k].c>0&&level[edge[k].to]==(level[now]+1))
        {
            c=dfs(edge[k].to,min(maxf-ret,edge[k].c),t);
            edge[k].c-=c;
            edge[k^1].c+=c;
            ret+=c;

            if(ret==maxf)   return  ret;
        }
    }
    --level[now];
    return ret;
}
int dinic(int s,int t)
{
    int  ans=0;
    while(makelevel(s,t))   ans+=dfs(s,INF,t);
    return ans;
}
void add(int u,int v,int c,int f)//有向边f为0 ,否则为 c
{
    edge[ip].to=v;edge[ip].c=c;edge[ip].next=head[u];head[u]=ip++;
    edge[ip].to=u;edge[ip].c=f;edge[ip].next=head[v];head[v]=ip++;
}
