#include<stdio.h>
#include<algorithm>
#include<queue>
#include<vector>
#include<stdlib.h>
#include<string>
#include<string.h>
#include<assert.h>
using namespace std;
#define MP make_pair
#define PB push_back
#define AA first
#define BB second
#define cmax(x,y) x=max(x,y)
#define cmin(x,y) x=min(x,y)
#define N 100010
#define M 100010
#define STEP 30
typedef long long LL;
typedef pair<int,int> PII;

const LL MOD=1000000007LL;

struct fnode
{
    int l,r;
    int v;
}pl[N*40];
int pcnt;
int n,m,b[N];
int vv[N],root[N],rcnt;
int depth[N], fa[STEP][N];

struct edgenode
{
    int u,v,next;
}edge[2*M];
int tot,last[N];

void addedge(int u,int v)
{
    edge[tot].u=u;
    edge[tot].v=v;
    edge[tot].next=last[u];
    last[u]=tot++;
}
void init()
{
    memset(depth, -1, sizeof(depth));
    memset(fa, 0 , sizeof(fa));
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
            depth[v] = depth[u] + 1;
            dfs_LCA(v);
        }
    }
}
int getLCA(int x, int y)
{
    int dif = abs(depth[x] - depth[y]);
    if(depth[x] < depth[y])
        swap(x, y);
    for(int i = STEP - 1; i >= 0; i--)
        if((1 << i) & dif)
        {
            dif -= (1 << i);
            x = fa[i][x];
        }
    for(int i = STEP - 1; i >= 0; i--)
        if(fa[i][x] != fa[i][y])
        {
            x = fa[i][x];
            y = fa[i][y];
        }
    if(x == y) return x;
    else return fa[0][x];
}
void solve()
{
    fa[0][n+1] = n+1;
    depth[0] = 0;
    dfs_LCA(n+1);
    for(int i = 1; i < STEP; i++)
        for(int j = 1; j <= n+1; j++)
            fa[i][j] = fa[i-1][fa[i-1][j]];
}
void build(int p,int pz,int py)
{
    int mid=(pz+py)>>1;
    pl[p].v=0;
    if(pz==py)return;
    build(pl[p].l=++pcnt,pz,mid);
    build(pl[p].r=++pcnt,mid+1,py);
}
void refresh(int p1,int p2,int pz,int py,int pos,int v)
{
    int mid=(pz+py)>>1;
    if(pz==py)
    {
        pl[p2].v+=v;
        return;
    }
    if(pos<=mid)
    {
        pl[p2].l=++pcnt;
        pl[p2].r=pl[p1].r;
        pl[pcnt]=pl[ pl[p1].l ];
        refresh(pl[p1].l,pl[p2].l,pz,mid,pos,v);
    }
    else
    {
        pl[p2].l=pl[p1].l;
        pl[p2].r=++pcnt;
        pl[pcnt]=pl[ pl[p1].r ];
        refresh(pl[p1].r,pl[p2].r,mid+1,py,pos,v);
    }
    int ll=pl[p2].l,rr=pl[p2].r;
    pl[p2].v=pl[ll].v+pl[rr].v;
}
int get_kth(int p1,int p2,int p3,int p4,int pz,int py,int k)
{
    int mid=(pz+py)>>1;
    if(pz==py)return b[pz];
    int ll1=pl[p1].l,ll2=pl[p2].l,ll3=pl[p3].l,ll4=pl[p4].l;
    int rr1=pl[p1].r,rr2=pl[p2].r,rr3=pl[p3].r,rr4=pl[p4].r;
    int del=pl[ll4].v-pl[ll3].v+pl[ll2].v-pl[ll1].v;
    if(del>=k)return get_kth(ll1,ll2,ll3,ll4,pz,mid,k);
    else return get_kth(rr1,rr2,rr3,rr4,mid+1,py,k-del);
}
void print(int p,int pz,int py)
{
    int mid=(pz+py)>>1;
    if(pz==py)printf("%d ",pl[p].v);
    else
    {
        print(pl[p].l,pz,mid);
        print(pl[p].r,mid+1,py);
    }
}
void getprint(int i)
{
    printf("u=%d\n",i);
    print(root[i],1,n);
    printf("\n");
}
void dfs_build(int u)
{
    int i,j,v;
    refresh(root[fa[0][u]],root[u]=++pcnt,1,n,vv[u],1);
    //getprint(u);
    for(j=last[u];j!=-1;j=edge[j].next)
    {
        v=edge[j].v;
        if(v==fa[0][u])continue;
        dfs_build(v);
    }
}
int query(int u,int v,int k)
{
    int lca=getLCA(u,v);
    return get_kth(root[fa[0][lca]],root[u],root[lca],root[v],1,n,k);
}
int main()
{
    int i,j;
    //printf("%d\n",sizeof pl);
    scanf("%d%d",&n,&m);
    init();
    for(i=1;i<=n;i++)
        scanf("%d",&vv[i]);
    for(i=1;i<=n;i++)
        b[i]=vv[i];
    sort(b+1,b+n+1);
    for(i=1;i<=n;i++)
        vv[i]=lower_bound(b+1,b+n+1,vv[i])-b;
    for(i=1;i<n;i++)
    {
        int s,t;
        scanf("%d%d",&s,&t);
        addedge(s,t);
        addedge(t,s);
    }
    addedge(n+1,1);
    addedge(1,n+1);
    solve();
    pcnt=0;rcnt=0;
    root[n+1]=1;
    build(1,1,n);
    dfs_build(1);
    /*
    for(i=1;i<=n+1;i++)
        getprint(i);
    //*/
    for(i=1;i<=m;i++)
    {
        int s,t,k;
        scanf("%d%d%d",&s,&t,&k);
        printf("%d\n",query(s,t,k));
    }
    return 0;
}
/*
8 6
105 2 9 3 8 5 7 7
1 2
1 3
1 4
3 5
3 6
3 7
4 8
1 7 1
1 7 2
1 7 3
4 8 1
1 8 2
1 2 1

8 5
105 2 9 3 8 5 7 7
1 2
1 3
1 4
3 5
3 6
3 7
4 8
2 5 1
2 5 2
2 5 3
2 5 4
7 8 2

8 5
7 7 7 7 8 8 8 8
1 2
1 3
1 4
3 5
3 6
3 7
4 8
2 5 1
2 5 2
2 5 3
2 5 4
7 8 2
*/
