#include<stdio.h>
#include<algorithm>
#include<stdlib.h>
#include<string.h>
#include<vector>
#include<assert.h>
#include<string>
#include<set>
using namespace std;
#define MP make_pair
#define PB push_back
#define AA first
#define BB second
#define cmax(x,y) x=max(x,y)
#define cmin(x,y) x=min(x,y)
typedef long long LL;
typedef pair<int,int> PII;

const LL MOD=1000000007LL;

struct edgenode
{
    int u,v,c,next;
}edge[20010];
int last[10010],tot;

vector<int>L[10010];
set<PII>S;

int tpsort[10010];
int Size,size[10010],Msize[10010];
int depth[10010];
int posc;
int done[10010];

int n,K;

void addedge(int u,int v,int c)
{
    edge[tot].u=u;
    edge[tot].v=v;
    edge[tot].c=c;
    edge[tot].next=last[u];
    last[u]=tot++;
}
int merge_sort(vector<int>&A,vector<int>&B)
{
    int i,j,n=A.size(),m=B.size(),ret=0;
    j=m-1;
    for(i=0;i<n;i++)
    {
        for(;j>=0 && B[j]+A[i]>K;j--);
        ret+=j+1;
    }
    j=0;
    for(i=0;i<n;i++)
    {
        while(j<m && B[j]<A[i])
        {
            tpsort[i+j]=B[j];
            j++;
        }
        tpsort[i+j]=A[i];
    }
    for(;j<m;j++)
        tpsort[n+j]=B[j];
    B.resize(1);
    A.resize(n+m);
    for(i=0;i<n+m;i++)
        A[i]=tpsort[i];
    return ret;
}
void dfs_size(int u,int fa)
{
    int i,j,v;
    size[u]=1;
    for(j=last[u];j!=-1;j=edge[j].next)
    {
        v=edge[j].v;
        if(v==fa || done[v])continue;
        dfs_size(v,u);
        size[u]+=size[v];
    }
}
void dfs_cent(int u,int fa)
{
    int i,j,v;
    Msize[u]=0;
    for(j=last[u];j!=-1;j=edge[j].next)
    {
        v=edge[j].v;
        if(v==fa || done[v])continue;
        dfs_cent(v,u);
        cmax(Msize[u],size[v]);
    }
    cmax(Msize[u],Size-size[u]);
    if(posc==-1 || Msize[posc]>Msize[u])posc=u;
}
void dfs_depth(int u,int fa,int ins)
{
    int i,j,v;
    for(j=last[u];j!=-1;j=edge[j].next)
    {
        v=edge[j].v;
        if(v==fa || done[v])continue;
        depth[v]=depth[u]+edge[j].c;
        L[ins].PB(depth[v]-depth[ins]);
        dfs_depth(v,u,ins);
    }
}
int gao(int u)
{
    int i,j,v,ret=0,cnow;
    dfs_size(u,-1);
    Size=size[u];
    posc=-1;
    dfs_cent(u,-1);
    cnow=posc;
    done[cnow]=1;
    //printf("cnow=%d\n\n",cnow);
    for(j=last[cnow];j!=-1;j=edge[j].next)
    {
        v=edge[j].v;
        if(done[v])continue;
        ret+=gao(v);
        //printf("0: %d ret=%d\n",v,ret);
    }
    S.clear();
    for(j=last[cnow];j!=-1;j=edge[j].next)
    {
        v=edge[j].v;
        if(done[v])continue;
        for(i=0;i<L[v].size();i++)
            if((L[v][i]+=edge[j].c)<=K)ret++;
        //printf("1: %d ret=%d\n",v,ret);
        S.insert(MP(L[v].size(),v));
    }
    while(S.size()>=2)
    {
        PII a=*(S.begin());
        S.erase(a);
        PII b=*(S.begin());
        S.erase(b);
        ret+=merge_sort(L[b.BB],L[a.BB]);
        //printf("2: %d+%d ret=%d\n",b.BB,a.BB,ret);
        S.insert(MP(L[b.BB].size(),b.BB));
    }
    done[cnow]=0;
    depth[u]=0;
    L[u].clear();
    L[u].PB(0);
    dfs_depth(u,-1,u);
    sort(L[u].begin(),L[u].end());
    //printf("END cnow=%d\n",cnow);
    return ret;
}
int main()
{
    int i,j,ans;
    while(1)
    {
        scanf("%d%d",&n,&K);
        if(n==0 && K==0)break;
        tot=0;
        for(i=1;i<=n;i++)
            last[i]=-1;
        for(i=1;i<n;i++)
        {
            int u,v,d;
            scanf("%d%d%d",&u,&v,&d);
            addedge(u,v,d);
            addedge(v,u,d);
        }
        for(i=1;i<=n;i++)
            done[i]=0;
        ans=gao(1);
        printf("%d\n",ans);
    }
    return 0;
}
