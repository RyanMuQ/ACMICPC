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
using std::vector;
using std::set;
using std::map;
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

const int N=100005;

struct edgenode
{
    int u,v,c,next;
}edge[N*2|1];
int tot,last[N];

void init(){
    tot=0;
    memset(last,-1,sizeof last);
}
void addedge(int u,int v,int c)
{
    edge[tot].u=u;
    edge[tot].v=v;
    edge[tot].c=c;
    edge[tot].next=last[u];
    last[u]=tot++;
}

struct Entry{
    int root,son,depth;
    Entry(){}
    Entry(int a,int b,int c):root(a),son(b),depth(c){}
};
Entry ent[N][19];
int cur[N];
int que[N],parent[N],depth[N],size[N],balance[N],n;
bool visit[N],state[N];
set<PII>exist[N];
map<int,vector<PII> >dist[N];
PII near[N];

unsigned int cal()
{
    int i;
    unsigned int ret=0;
    map<int,vector<PII> >::iterator it;
    ret+=sizeof(ent);
    for(i=0;i<N;i++)
        ret+=exist[i].size()*8;
    for(i=0;i<N;i++)
        for(it=dist[i].begin();it!=dist[i].end();++it)
            ret+=(*it).BB.size()*8;
    return ret;
}

int split(int root){
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
    ent[root][cur[root]++]=Entry(root,-1,0);
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
            ent[u][cur[u]++]=Entry(root,v,depth[u]);
        }
        split(v);
    }
    visit[root]=0;
    return root;
}
int main(){
    int i,j,n,ans;
    while(~scanf("%d",&n))
    {
        init();
        for(i=1;i<n;i++)
        {
            int s,t,c;
            scanf("%d%d%d",&s,&t,&c);
            addedge(s,t,c);
            addedge(t,s,c);
        }
        for(i=1;i<=n;i++)
        {
            int tp;
            scanf("%d",&tp);
            if(tp)state[i]=1;
            else state[i]=0;
        }
        for(i=1;i<=n;i++)
        {
            visit[i]=0;
            cur[i]=0;
            exist[i].clear();
            dist[i].clear();
        }
        split(1);
        for(i=1;i<=n;i++)
        {
            if(!state[i])continue;
            for(j=0;j<cur[i];j++)
            {
                Entry &th=ent[i][j];
                exist[th.root].insert(MP(th.depth,i));
            }
        }
        for(i=1;i<=n;i++)
        {
            near[i]=MP(MOD,-1);
            for(j=0;j<cur[i];j++)
            {
                Entry &th=ent[i][j];
                if(!exist[th.root].empty())
                {
                    PII tp=*exist[th.root].begin();
                    tp.AA+=th.depth;
                    cmin(near[i],tp);
                }
            }
        }
        for(i=1;i<=n;i++)
            exist[i].clear();
        for(i=1;i<=n;i++)
        {
            for(j=0;j<cur[i];j++)
            {
                Entry &th=ent[i][j];
                dist[th.root][-1].PB(MP(near[i].AA-th.depth,near[i].BB));
                if(th.son!=-1)dist[th.root][th.son].PB(MP(near[i].AA-th.depth,near[i].BB));
            }
        }
        for(i=1;i<=n;i++)
        {
            map<int,vector<PII> >::iterator it;
            for(it=dist[i].begin();it!=dist[i].end();++it)
            {
                vector<PII>&th=(*it).BB;
                sort(th.begin(),th.end());
            }
        }
        ans=0;
        for(i=1;i<=n;i++)
        {
            if(state[i])continue;
            int ret=0;
            for(j=0;j<cur[i];j++)
            {
                Entry &th=ent[i][j];
                vector<PII>* tp=&dist[th.root][-1];
                int t1=(*tp).end()-upper_bound((*tp).begin(),(*tp).end(),MP(th.depth,i));
                int t2=0;
                if(th.son!=-1)
                {
                    tp=&dist[th.root][th.son];
                    t2=(*tp).end()-upper_bound((*tp).begin(),(*tp).end(),MP(th.depth,i));
                }
                ret+=t1-t2;
            }
            cmax(ans,ret);
        }
        printf("%d\n",ans);
        for(i=1;i<=n;i++)
            dist[i].clear();
    }
    return 0;
}
