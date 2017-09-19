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
typedef long long LL;
typedef std::pair<int, int> PII;
#define PB push_back
#define MP std::make_pair
#define AA first
#define BB second
#define OP begin()
#define ED end()
#define SZ size()
#define SQ(x) ((x)*(x))
#define SSP system("pause")
#define cmin(x,y) x=std::min(x,y)
#define cmax(x,y) x=std::max(x,y)
const double eps=1e-8;
const double INF=1e20;
const double PI=acos( -1. );
const int MXN = 50;
const LL MOD = 1000000007;
const LL MAXN=100010;
const LL MAXM=200010;

struct edgenode
{
    int u,v,next;
};
struct fnode
{
    int l,r,cnt;
}pl[MAXN*35];
struct Graph
{
    int tot,last[MAXN];
    edgenode edge[MAXM];
    void init()
    {
        tot=0;
        memset(last,-1,sizeof last);
    }
    void addedge(int u,int v)
    {
        edge[tot].u=u;
        edge[tot].v=v;
        edge[tot].next=last[u];
        last[u]=tot++;
    }
}G;

int n,m,pcnt,root[MAXN];
PII range[MAXN];
int vv[MAXN],b[MAXN],F[MAXN];

void getint(int &a)
{
    char c;
    while(1)
    {
        c=getchar();
        if(c>='0' && c<='9')break;
    }
    a=c-'0';
    while(1)
    {
        c=getchar();
        if(c==-1 || c=='\n' || c==' ')break;
        a=a*10+c-'0';
    }
}
void build(int p,int pz,int py)
{
    pl[p].cnt=0;
    if(pz==py)return;
    int mid=(pz+py)>>1;
    build(pl[p].l=++pcnt,pz,mid);
    build(pl[p].r=++pcnt,mid+1,py);
}
void refresh(int p,int p2,int pz,int py,int pos)
{
    int mid=(pz+py)>>1;
    if(pz==py)
    {
        pl[p].cnt++;
        return;
    }
    if(pos<=mid)
    {
        pl[pl[p].l=++pcnt]=pl[pl[p2].l];
        pl[p].r=pl[p2].r;
        refresh(pl[p].l,pl[p2].l,pz,mid,pos);
    }
    else
    {
        pl[p].l=pl[p2].l;
        pl[pl[p].r=++pcnt]=pl[pl[p2].r];
        refresh(pl[p].r,pl[p2].r,mid+1,py,pos);
    }
    int ll=pl[p].l,rr=pl[p].r;
    pl[p].cnt=pl[ll].cnt+pl[rr].cnt;
}
void dfs_range(int u,int fa,int &now)
{
    int i,j,v;
    range[u].AA=now;
    pl[root[now]=++pcnt]=pl[root[now-1]];
    refresh(root[now],root[now-1],1,n,vv[u]);
    //printf("u=%d INSERT %d %d\n",u,now,vv[u]);
    for(j=G.last[u];j!=-1;j=G.edge[j].next)
    {
        v=G.edge[j].v;
        if(v==fa)continue;
        now++;
        dfs_range(v,u,now);
    }
    range[u].BB=now;
}
int query(int p,int p2,int pz,int py,int k)
{
    int mid=(pz+py)>>1;
    if(pz==py)return pz;
    int ll=pl[p].l,ll2=pl[p2].l;
    int rr=pl[p].r,rr2=pl[p2].r;
    int cc=pl[ll].cnt-pl[ll2].cnt;
    if(cc>=k)return query(ll,ll2,pz,mid,k);
    else return query(rr,rr2,mid+1,py,k-cc);
}

int main(){
    int i,j,tp;
    getint(n);
    for(i=1;i<=n;i++)
        getint(vv[i]);
    for(i=1;i<=n;i++)
        b[i]=vv[i];
    std::sort(b+1,b+n+1);
    for(i=1;i<=n;i++)
    {
        vv[i]=std::lower_bound(b+1,b+n+1,vv[i])-b;
        F[vv[i]]=i;
    }
    G.init();
    for(i=1;i<n;i++)
    {
        int s,t;
        getint(s);
        getint(t);
        G.addedge(s,t);
        G.addedge(t,s);
    }
    pcnt=0; 
    build(root[0]=++pcnt,1,n);
    dfs_range(1,-1,tp=1);
//    for(i=1;i<=n;i++)
//    {
//        printf("u=%d\n",i);
//        for(j=1;j<=5;j++)
//            printf("%d ",query(root[range[i].BB],root[range[i].AA-1],1,n,j));
//        printf("\n");
//    }
    getint(m);
    for(i=1;i<=m;i++)
    {
        int x,k;
        getint(x);
        getint(k);
        int ans=query(root[range[x].BB],root[range[x].AA-1],1,n,k);
        //printf("ans=%d\t",ans);
        printf("%d\n",F[ans]);
    }
    return 0;
}
