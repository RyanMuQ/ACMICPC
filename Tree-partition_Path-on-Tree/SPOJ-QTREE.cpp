#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<vector>
#include<string>
#include<math.h>
using namespace std;
#define cmax(x,y) x=max(x,y)
#define cmin(x,y) x=min(x,y)
#define AA first
#define BB second
#define MP make_pair
typedef long long LL;
typedef pair<int,int> PII;

const LL MOD=1000000007LL;

struct inputedge
{
    int u,v;
}iedge[10010];
struct edgenode
{
    int u,v,c,next;
}edge[20010];
int tot,last[10010];
int depth[10010],size[10010],be[10010],No[10010],top[10010];
int fa[10010],lv[10010];

struct node
{
    int z,y,l,r;
    int v;
}pl[40010];
int pcnt,n;

void addedge(int u,int v,int c)
{
    edge[tot].u=u;
    edge[tot].v=v;
    edge[tot].c=c;
    edge[tot].next=last[u];
    last[u]=tot++;
}
void build(int p,int zz,int yy)
{
    int mid=(zz+yy)>>1;
    pl[p].v=-1;
    pl[p].z=zz;
    pl[p].y=yy;
    if(zz==yy)return;
    build(pl[p].l=++pcnt,zz,mid);
    build(pl[p].r=++pcnt,mid+1,yy);
}
void refresh(int p,int pos,int vv)
{
    int mid=(pl[p].z+pl[p].y)>>1;
    if(pl[p].z==pl[p].y)
    {
        pl[p].v=vv;
        return;
    }
    if(pos<=mid)refresh(pl[p].l,pos,vv);
    else refresh(pl[p].r,pos,vv);
    pl[p].v=max(pl[pl[p].l].v,pl[pl[p].r].v);
}
int query(int p,int zz,int yy)
{
    int mid=(pl[p].z+pl[p].y)>>1;
    if(pl[p].z==zz && pl[p].y==yy)return pl[p].v;
    if(yy<=mid)return query(pl[p].l,zz,yy);
    else if(mid<zz)return query(pl[p].r,zz,yy);
    else
    {
        int t1=query(pl[p].l,zz,mid);
        int t2=query(pl[p].r,mid+1,yy);
        return max(t1,t2);
    }
}
void init()
{
    pcnt=0;
    tot=0;
    memset(last,-1,sizeof last);
}
void dfs_ds(int u,int ffa)
{
    int j,v;
    size[u]=1;
    for(j=last[u];j!=-1;j=edge[j].next)
    {
        v=edge[j].v;
        if(v==ffa)continue;
        lv[v]=edge[j].c;
        fa[v]=u;
        depth[v]=depth[u]+1;
        dfs_ds(v,u);
        size[u]+=size[v];
    }
}
void spilt(int u,int Top,int ffa)
{
    int i,j,v,Max=-1;
    for(j=last[u];j!=-1;j=edge[j].next)
    {
        v=edge[j].v;
        if(v==ffa)continue;
        if(Max==-1 || size[Max]<size[v])Max=v;
    }
    if(Max==-1)
    {
        top[u]=Top;
        No[Top]=0;
        No[u]=depth[u]-depth[Top];
        build(be[u]=++pcnt,No[Top]-1,No[u]);
        refresh(be[u],No[u],lv[u]);
        return;
    }
    spilt(Max,Top,u);
    No[u]=No[Max]-1;
    be[u]=be[Max];
    top[u]=top[Max];
    refresh(be[u],No[u],lv[u]);
    for(j=last[u];j!=-1;j=edge[j].next)
    {
        v=edge[j].v;
        if(v==ffa || v==Max)continue;
        spilt(v,v,u);
    }
}
void update(int u,int c)
{
    if(top[u]==u)lv[u]=c;
    else refresh(be[u],No[u],c);
}
int Query(int u,int v)
{
    int ret=-1;
    while(1)
    {
        if(be[u]==be[v])
        {
            if(No[u]>No[v])swap(u,v);
            if(No[u]<No[v])cmax(ret,query(be[u],No[u]+1,No[v]));
            return ret;
        }
        if(depth[top[u]]>depth[top[v]])swap(u,v);
        if(v!=top[v])cmax(ret,query(be[v],1,No[v]));
        v=top[v];
        cmax(ret,lv[v]);
        v=fa[v];
    }
}
int main()
{
    int i,j,T;
    char ss[10];
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
            iedge[i].u=s;
            iedge[i].v=t;
        }
        fa[1]=-1;
        depth[1]=0;
        dfs_ds(1,-1);
        spilt(1,1,-1);
        while(1)
        {
            scanf("%s",ss);
            if(ss[0]=='D')break;
            if(ss[0]=='Q')
            {
                int s,t;
                scanf("%d%d",&s,&t);
                printf("%d\n",Query(s,t));
            }
            else if(ss[0]=='C')
            {
                int k,c;
                scanf("%d%d",&k,&c);
                int s=iedge[k].u,t=iedge[k].v;
                if(fa[s]==t)swap(s,t);
                //printf("t=%d\n",t);
                update(t,c);
            }
        }
    }
    return 0;
}

