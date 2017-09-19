#include<stdio.h>
#include<algorithm>
#include<vector>
#include<assert.h>
using namespace std;
typedef long long LL;
typedef pair<int,int> PII;
#define MP make_pair
#define PB push_back
#define AA first
#define BB second
#define cmax(x,y) x=max(x,y)
#define cmin(x,y) x=min(x,y)

const int MOD=1000000007;
const int Maxn=205;
const int INF=10007;
struct edgenode
{
    int v,c,next;
}edge[Maxn<<1|1];
int tot,last[Maxn];
int dp[Maxn][Maxn][Maxn],Max[Maxn];
int ord[Maxn],size[Maxn],n,m;
int state[Maxn];
int dis[Maxn][Maxn],Cnt;
PII be[Maxn],range[Maxn];

void addedge(int u,int v,int c)
{
    edge[tot].v=v;
    edge[tot].c=c;
    edge[tot].next=last[u];
    last[u]=tot++;
}
void dfsorder(int u,int fa)
{
    range[u].AA=++Cnt;
    ord[Cnt]=u;
    size[u]=1;
    for(int j=last[u];j!=-1;j=edge[j].next)
    {
        int v=edge[j].v;
        if(v==fa)continue;
        dfsorder(v,u);
        size[u]+=size[v];
    }
    range[u].BB=Cnt;
}
void getdp(int u,int fa)
{
    int i,j,k,v,top;
    for(i=0;i<=n;i++)
        for(j=1;j<=n;j++)
            dp[u][i][j]=-INF;
    dp[u][0][be[u].BB]=0;
    for(i=1;i<=n;i++)
        if(!state[i] && MP(dis[u][i],i)<be[u])
        {
            if(i==u)dp[u][1][i]=1;
            else dp[u][0][i]=1;
        }
    top=1;
    for(int _=last[u];_!=-1;_=edge[_].next)
    {
        v=edge[_].v;
        if(v==fa)continue;
        getdp(v,u);
        for(k=0;k<=size[v];k++)
        {
            Max[k]=-INF;
            for(i=range[v].AA;i<=range[v].BB;i++)
                cmax(Max[k],dp[v][k][ord[i]]);
        }
        for(j=top;j>=0;j--)
        {
            for(k=size[v];k>=1;k--)
            {
                dp[u][j+k][i]=-INF;
                for(i=1;i<=n;i++)
                    if(range[i].AA>=range[v].AA && range[i].AA<=range[v].BB)
                        cmax(dp[u][j+k][i],dp[u][j][i]+dp[v][k][i]);
                    else cmax(dp[u][j+k][i],dp[u][j][i]+max(Max[k],dp[v][k][i]));
            }
            for(i=1;i<=n;i++)
                if(range[i].AA>=range[v].AA && range[i].AA<=range[v].BB)
                    dp[u][j][i]+=dp[v][k][i];
                else dp[u][j][i]+=max(Max[k],dp[v][k][i]);
        }
        top+=size[v];
    }
}

int main()
{
    int i,j,k,ans;
    while(~scanf("%d%d",&n,&m))
    {
        tot=0;
        for(i=1;i<=n;i++)
            last[i]=-1;
        for(i=1;i<=n;i++)
            for(j=1;j<=n;j++)
                dis[i][j]=MOD;
        for(i=1;i<=n;i++)
            dis[i][i]=0;
        for(i=1;i<n;i++)
        {
            int s,t,c;
            scanf("%d%d%d",&s,&t,&c);
            addedge(s,t,c);
            addedge(t,s,c);
            dis[s][t]=dis[t][s]=c;
        }
        for(k=1;k<=n;k++)
            for(i=1;i<=n;i++)
                for(j=1;j<=n;j++)
                    cmin(dis[i][j],dis[i][k]+dis[k][j]);
        for(i=1;i<=n;i++)
            scanf("%d",&state[i]);
        for(i=1;i<=n;i++)
        {
            be[i]=MP(MOD,n+1);
            for(j=1;j<=n;j++)
                if(state[j])cmin(be[i],MP(dis[i][j],j));
        }
        Cnt=0;
        dfsorder(1,-1);
        getdp(1,-1);
        ans=0;
        for(j=1;j<=n;j++)
            for(i=1;i<=m;i++)
                cmax(ans,dp[1][i][j]);
        printf("%d\n",ans);
    }
    return 0;
}
