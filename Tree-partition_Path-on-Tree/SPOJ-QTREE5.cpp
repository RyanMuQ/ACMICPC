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

multiset<int>S[N];
bool state[N];
struct edgenode
{
    int u,v,next;
}edge[N<<1|1];
int tot,last[N];

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

struct Entry
{
    int root,son,depth;
    Entry(){}
    Entry(int a,int b,int c):root(a),son(b),depth(c){}
};
vector<Entry>ent[N];
int que[N],parent[N],depth[N],size[N],balance[N],n;
bool visit[N];

int split(int root)
{
    int i,j,u,v,Max,ed=-1;
    que[++ed]=root;
    parent[root]=root;
    size[root]=1;
    for(i=0;i<=ed;i++)
    {
        u=que[i];
        for(j=last[u];j!=-1;j=edge[j].next)
        {
            v=edge[j].v;
            if(visit[v] || parent[u]==v)continue;
            parent[v]=u;
            size[v]=1;
            que[++ed]=v;
        }
    }
    for(i=ed;i>0;i--)
        size[parent[que[i]]]+=size[que[i]];
    for(i=0;i<=ed;i++)
        balance[que[i]]=size[root]-size[que[i]];
    for(i=ed;i>0;i--)
        cmax(balance[parent[que[i]]],size[que[i]]);
    for(i=0;i<=ed;i++)
        if(balance[root]>balance[que[i]])root=que[i];
    visit[root]=1;
    depth[root]=0;
    ent[root].PB(Entry(root,-1,0));
    for(int _=last[root];_!=-1;_=edge[_].next)
    {
        v=edge[_].v;
        if(visit[v])continue;
        parent[v]=v;
        depth[v]=1;
        ed=-1;
        que[++ed]=v;
        for(i=0;i<=ed;i++)
        {
            u=que[i];
            for(j=last[u];j!=-1;j=edge[j].next)
            {
                int vv=edge[j].v;
                if(parent[u]==vv || visit[vv])continue;
                depth[vv]=depth[u]+1;
                parent[vv]=u;
                que[++ed]=vv;
            }
        }
        for(i=0;i<=ed;i++)
        {
            u=que[i];
            ent[u].PB(Entry(root,v,depth[u]));
        }
        split(v);
    }
    visit[root]=0;
    return root;
}

int main(){
    int i,j,q;
    scanf("%d",&n);
    init();
    for(i=1;i<n;i++)
    {
        int s,t,c;
        scanf("%d%d",&s,&t);
        addedge(s,t);
        addedge(t,s);
    }
    for(i=0;i<=n;i++)
    {
        ent[i].clear();
        S[i].clear();
        state[i]=0;
        visit[i]=0;
    }
    split(1);
    scanf("%d",&q);
    for(j=1;j<=q;j++)
    {
        int buff,v;
        scanf("%d%d",&buff,&v);
        if(buff==0)
        {
            state[v]^=1;
            for(i=0;i<ent[v].size();i++)
            {
                Entry &tp=ent[v][i];
                if(state[v])S[tp.root].insert(tp.depth);
                else S[tp.root].erase(S[tp.root].find(tp.depth));
            }
        }
        else
        {
            int ans=MOD;
            for(i=0;i<ent[v].size();i++)
            {
                Entry &tp=ent[v][i];
                if(S[tp.root].size()==0)continue;
                cmin(ans,tp.depth+*S[tp.root].begin());
            }
            if(ans>=MOD)ans=-1;
            printf("%d\n",ans);
        }
    }
    return 0;
}
