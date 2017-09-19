#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<vector>
#include<string>
#include<math.h>
#include<algorithm>
using namespace std;
#define cmax(x,y) x=max(x,y)
#define cmin(x,y) x=min(x,y)
#define AA first
#define BB second
#define MP make_pair
typedef long long LL;
typedef pair<int,int> PII;

const LL MOD=1000000007LL;
const int N=10010;
const int K=100000;
const int STEP=30;

struct edgenode
{
    int u,v,next,c;
}edge[N*2];
int tot,last[N];
int n;
int depth[N], fa[STEP][N], dis[STEP][N];

void addedge(int x, int y,int c)
{
    edge[tot].u = x;
    edge[tot].v = y;
    edge[tot].c=c;
    edge[tot].next = last[x];
    last[x] = tot++;
}
void init()
{
    memset(depth, -1, sizeof(depth));
    memset(fa, 0 , sizeof(fa));
    memset(dis, 0 , sizeof(dis));
    memset(last, -1, sizeof(last));
    tot = 0;
}
void dfs_LCA(int u)
{
    int v;
    for(int j = last[u]; -1 != j; j = edge[j].next)
    {
        v = edge[j].v;
        if(fa[0][v] == 0)
        {
            fa[0][v] = u;
            dis[0][v] = edge[j].c;
            depth[v] = depth[u] + 1;
            dfs_LCA(v);
        }
    }
}
PII getLCA(int x, int y)
{
    int dif = abs(depth[x] - depth[y]);
    int ret=0;
    if(depth[x] < depth[y])
        swap(x, y);
    for(int i = STEP - 1; i >= 0; i--)
        if((1 << i) & dif)
        {
            dif -= (1 << i);
            ret+=dis[i][x];
            x = fa[i][x];
        }
    for(int i = STEP - 1; i >= 0; i--)
        if(fa[i][x] != fa[i][y])
        {
            ret+=dis[i][x];
            ret+=dis[i][y];
            x = fa[i][x];
            y = fa[i][y];
        }
    if(x == y) return MP(x,ret);
    else return MP(fa[0][x],ret+dis[0][x]+dis[0][y]);
}
void solve()
{
    fa[0][1] = 1;
    depth[1] = 0;
    dfs_LCA(1);
    for(int i = 1; i < STEP; i++)
        for(int j = 1; j <= n; j++)
        {
            fa[i][j] = fa[i-1][fa[i-1][j]];
            dis[i][j]=dis[i-1][fa[i-1][j]]+dis[i-1][j];
        }
}
int find_kthfa(int u,int k)
{
    for(int i=STEP-1;i>=0;i--)
        if((1<<i)<=k)
        {
            u=fa[i][u];
            k-=1<<i;
        }
    return u;
}
int Query_kth(int u,int v,int k)
{
    int lca=getLCA(u,v).AA;
    if(depth[u]-depth[lca]+1>=k)return find_kthfa(u,k-1);
    else
    {
        k-=depth[u]-depth[lca]+1;
        k=depth[v]-depth[lca]-k;
        return find_kthfa(v,k);
    }
}
int Query_dis(int u,int v)
{
    return getLCA(u,v).BB;
}

int main()
{
    //freopen("2.txt","r",stdin);
    int i,j,T;
    scanf("%d",&T);
    while(T--)
    {
        scanf("%d",&n);
        init();
        for(i=1;i<n;i++)
        {
            int s,t,c;
            scanf("%d%d%d",&s,&t,&c);
            addedge(s,t,c);
            addedge(t,s,c);
        }
        solve();
//        for(i=1;i<=n;i++)
//            printf("%d ",dis[0][i]);
//        printf("\n");
        while(1)
        {
            char ss[10];
            scanf("%s",ss);
            if(ss[1]=='O')break;
            if(ss[1]=='I')
            {
                int s,t;
                scanf("%d%d",&s,&t);
                printf("%d\n",Query_dis(s,t));
            }
            else if(ss[1]=='T')
            {
                int s,t,k;
                scanf("%d%d%d",&s,&t,&k);
                printf("%d\n",Query_kth(s,t,k));
            }
        }
        printf("\n");
    }
    return 0;
}
