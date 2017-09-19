#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <bitset>
#include <cctype>
#include <cstdio>
#include <string>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long LL;
typedef pair<int, int> PII;
typedef pair<int,PII> PIII;
#define PB push_back
#define MP make_pair
#define AA first
#define BB second
#define OP begin()
#define ED end()
#define SZ size()
#define SQ(x) ((x)*(x))
#define SSP system("pause")
#define cmin(x,y) x=min(x,y)
#define cmax(x,y) x=max(x,y)
const double eps=1e-8;
const double INF=1e20;
const double PI=acos( -1. );
const int MXN = 50;
const LL MOD = 1000000007;

const int N=100011;

struct edgenode
{
    int u,v,c,next;
}edge[N<<1|1];
int tot,last[N];
int depth[N],size[N],No[N],top[N],slen[N],root[N];
int parent[N],state[N],tsum[N][2];
map<int,int>Map[N];

struct node
{
    int l,r;
    int v[3];
}pl[N<<2|1];
int pcnt,n;
void addedge(int u,int v)
{
    edge[tot].u=u;
    edge[tot].v=v;
    edge[tot].c=1;
    edge[tot].next=last[u];
    last[u]=tot++;
}

void build(int p,int pz,int py)
{
    for(int i=0;i<3;i++)
        pl[p].v[i]=0;
    if(pz==py)return;
    int mid=(pz+py)>>1;
    build(pl[p].l=++pcnt,pz,mid);
    build(pl[p].r=++pcnt,mid+1,py);
}
void refresh(int p,int pz,int py,int pos,int tp,int vv)
{
    int mid=(pz+py)>>1;
    if(pz==py)
    {
        pl[p].v[tp]+=vv;
        return;
    }
    if(pos<=mid)refresh(pl[p].l,pz,mid,pos,tp,vv);
    else refresh(pl[p].r,mid+1,py,pos,tp,vv);
    pl[p].v[tp]=pl[pl[p].l].v[tp]+pl[pl[p].r].v[tp];
}
int query(int p,int pz,int py,int zz,int yy,int tp)
{
    int mid=(pz+py)>>1;
    if(pz==zz && py==yy)return pl[p].v[tp];
    if(yy<=mid)return query(pl[p].l,pz,mid,zz,yy,tp);
    else if(mid<zz)return query(pl[p].r,mid+1,py,zz,yy,tp);
    else
    {
        int t1=query(pl[p].l,pz,mid,zz,mid,tp);
        int t2=query(pl[p].r,mid+1,py,mid+1,yy,tp);
        return t1+t2;
    }
}
void getblock(int p,int pz,int py,int zz,int yy,vector<PIII>&L)
{
    int mid=(pz+py)>>1;
    if(pz==zz && py==yy)
    {
        L.PB(MP(p,MP(pz,py)));
        return;
    }
    if(yy<=mid)getblock(pl[p].l,pz,mid,zz,yy,L);
    else if(mid<zz)getblock(pl[p].r,mid+1,py,zz,yy,L);
    else
    {
        getblock(pl[p].l,pz,mid,zz,mid,L);
        getblock(pl[p].r,mid+1,py,mid+1,yy,L);
    }
}
int querylow(int p,int pz,int py,int ll,int tp)
{
    int i,sum=0;
    vector<PIII>L;
    L.clear();
    getblock(p,pz,py,ll,py,L);
    for(i=0;i<L.size();i++)
    {
        if(sum+pl[L[i].AA].v[2]!=tp*(L[i].BB.BB-ll+1))
        {
            p=L[i].AA;
            pz=L[i].BB.AA;
            py=L[i].BB.BB;
            while(pz<py)
            {
                int mid=(pz+py)>>1;
                if(sum+pl[pl[p].l].v[2]!=tp*(mid-ll+1))
                {
                    p=pl[p].l;pz=pz;py=mid;
                }
                else
                {
                    sum+=pl[pl[p].l].v[2];
                    p=pl[p].r;pz=mid+1;py=py;
                }
            }
            return py-1;
        }
        sum+=pl[L[i].AA].v[2];
    }
    return py;
}
int queryhigh(int p,int pz,int py,int rr,int tp)
{
    int i,sum=0;
    vector<PIII>L;
    L.clear();
    getblock(p,pz,py,pz,rr,L);
    for(i=L.size()-1;i>=0;i--)
    {
        if(sum+pl[L[i].AA].v[2]!=tp*(rr-L[i].BB.AA+1))
        {
            p=L[i].AA;
            pz=L[i].BB.AA;
            py=L[i].BB.BB;
            while(pz<py)
            {
                int mid=(pz+py)>>1;
                if(sum+pl[pl[p].r].v[2]!=tp*(rr-(mid+1)+1))
                {
                    p=pl[p].r;pz=mid+1;py=py;
                }
                else
                {
                    sum+=pl[pl[p].r].v[2];
                    p=pl[p].l;pz=pz;py=mid;
                }
            }
            return pz+1;
        }
        sum+=pl[L[i].AA].v[2];
    }
    return pz;
}
void init(){
    pcnt=0;
    tot=0;
    memset(last,-1,sizeof last);
}
void dfs_ds(int u) {
    int j,v;
    size[u]=1;
    for(j=last[u];j!=-1;j=edge[j].next)
    {
        v=edge[j].v;
        if(v==parent[u])continue;
        parent[v]=u;
        depth[v]=depth[u]+1;
        dfs_ds(v);
        size[u]+=size[v];
    }
}
void split(int u,int Top) {
    int i,j,v,Max=-1;
    top[u]=Top;
    for(j=last[u];j!=-1;j=edge[j].next)
    {
        v=edge[j].v;
        if(v==parent[u])continue;
        if(Max==-1 || size[Max]<size[v])Max=v;
    }
    if(Max==-1)
    {
        No[u]=depth[u]-depth[Top]+1;
        slen[u]=No[u];
        tsum[Top][0]=size[Top];
        tsum[Top][1]=0;
        build(root[u]=++pcnt,1,slen[u]);
        return;
    }
    split(Max,Top);
    No[u]=No[Max]-1;
    root[u]=root[Max];
    slen[u]=slen[Max];
    refresh(root[u],1,slen[u],No[u],0,size[u]-size[Max]-1);
    for(j=last[u];j!=-1;j=edge[j].next)
    {
        v=edge[j].v;
        if(v==parent[u] || v==Max)continue;
        split(v,v);
    }
}
int Query(int u)
{
    while(u!=1)
    {
        int pos=queryhigh(root[u],1,slen[u],No[u],state[u]);
        if(pos>1)
        {
            u=Map[top[u]][pos];
            break;
        }
        u=top[u];
        if(state[parent[u]]!=state[u])break;
        u=parent[u];
    }
    int pos=querylow(root[u],1,slen[u],No[u],state[u]);
    return pos-No[u]+1+query(root[u],1,slen[u],No[u],pos,state[u]);
}
void Reverse(int u)
{
    int s0,s1,s00,s01;
    state[u]^=1;
    refresh(root[u],1,slen[u],No[u],2,state[u]-(state[u]^1));
    while(top[u]!=1)
    {
        if(state[top[u]]==0)
        {
            s0=querylow(root[u],1,slen[u],1,0);
            s0=query(root[u],1,slen[u],1,s0,0)+s0;
            s1=0;
        }
        else
        {
            s0=0;
            s1=querylow(root[u],1,slen[u],1,1);
            s1=query(root[u],1,slen[u],1,s1,1)+s1;
        }
        s00=tsum[top[u]][0];
        s01=tsum[top[u]][1];
        tsum[top[u]][0]=s0;
        tsum[top[u]][1]=s1;
        u=top[u];
        u=parent[u];
        refresh(root[u],1,slen[u],No[u],0,s0-s00);
        refresh(root[u],1,slen[u],No[u],1,s1-s01);
    }
}
int main(){
    int i,j,q;
    scanf("%d",&n);
    init();
    for(i=1;i<n;i++)
    {
        int s,t;
        scanf("%d%d",&s,&t);
        addedge(s,t);
        addedge(t,s);
    }
    size[1]=1;
    parent[1]=1;
    dfs_ds(1);
    split(1,1);
    for(i=1;i<=n;i++)
    {
        Map[i].clear();
        state[i]=0;
    }
    for(i=1;i<=n;i++)
        Map[top[i]][No[i]]=i;
    scanf("%d",&q);
    for(j=1;j<=q;j++)
    {
        int buff,u;
        scanf("%d%d",&buff,&u);
        if(buff==0)printf("%d\n",Query(u));
        else Reverse(u);
    }
    return 0;
}
