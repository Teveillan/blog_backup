int dfn[N],low[N],fa[N];
int stack[N];
int top,time1,sig;
void dfs(int pos,int pre)
{
    stack[++top]=pos;
    dfn[pos]=low[pos]=++time1;
    for(int p=head[pos];p!=-1;p=edge[p].next)
    {
        int to=edge[p].to;
        if(p==pre) continue; 	 //点双连通 if(to==pre) continue;
        if(dfn[to]) low[pos]=min(low[pos],low[to]);
        else
        {
            dfs(to,p^1); 	 //点双连通 dfs(to,pos)
            low[pos]=min(low[pos],low[to]);
            if(dfn[pos]<low[to]) //点双连通 dfn[pos]<=low[to]，根节点按题目特殊处理
            {
                ++sig;
                do
                {
                    fa[ stack[top] ]=sig;
                }while(stack[top]--!=to);
            }
        }
    }
}
void solve(int n)
{
    memset(dfn,0,sizeof(dfn));
    time1=sig=top=0;
    FOR(i,1,n) if(!dfn[i]) dfs(i,-1);
}

