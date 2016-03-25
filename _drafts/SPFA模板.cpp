#define INF 1e9
#define N 
#define M   
typedef int type;
type dis[N];
bool visit[N];
int que[N];
int head[N],ip;
struct Edge
{
    int to,next;
    type c;
}edge[M];
void add(int u,int v,type c)
{
    edge[ip].to=v; edge[ip].c=c; edge[ip].next=head[u]; head[u]=ip++;
}
void spfa(int start,int numpoint)
{
    memset(visit,0,sizeof(visit));
    FOR(i,0,numpoint) dis[i]=INF;
    int front=-1,tail=-1;
    dis[start]=0; visit[start]=1;  que[++tail]=start;
    int top,to;
    type temp;
    while(front!=tail)
    {
        if(++front>numpoint) front-=numpoint;
        top=que[front];  visit[top]=0;
        for(int p=head[top];p!=-1;p=edge[p].next)
        {
            to=edge[p].to; temp=dis[top]+edge[p].c;
            if(dis[to]>temp)
            {
                dis[to]=temp;
                if(!visit[to])
                {
                    if(++tail>numpoint) tail-=numpoint;
                    que[tail]=to;
                    visit[to]=1;
                }
            }
        }
    }
}
