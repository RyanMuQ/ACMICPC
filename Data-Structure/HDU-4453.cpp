#include<stdio.h>
#include<iostream>
#include<algorithm>
#include<assert.h>
using namespace std;

const int N=100010;
const int M=100010;

struct Listnode
{
    int v;
    int next,last;
} L[N+M];
int tot,del;
bool Rev;

struct List
{
    int Head,End;
    int p1,p2;
    void init()
    {
        Head=tot++;
        End=tot++;
        L[Head].next=End;
        L[Head].last=-1;
        L[End].last=Head;
        L[End].next=-1;
    }
    void Delete(int p1,int p2)
    {
        L[L[p1].last].next=L[p2].next;
        L[L[p2].next].last=L[p1].last;
    }
    void Insert(int p1,int p2,int q)
    {
        L[p2].next=L[q].next;
        L[L[q].next].last=p2;
        L[q].next=p1;
        L[p1].last=q;
    }
    void Insert(int vv,int q)
    {
        L[tot].v=vv;
        Insert(tot,tot,q);
        tot++;
    }
} A;

inline void moveleft()
{
    if(Rev)
    {
        int t=L[A.Head].next;
        A.Delete(t,t);
        A.Insert(t,t,L[A.p1].last);
    }
    A.p1=L[A.p1].last;
    A.p2=L[A.p2].last;
    L[A.p2].v+=del;
}
inline void moveright()
{
    L[A.p2].v-=del;
    A.p2=L[A.p2].next;
    A.p1=L[A.p1].next;
    if(Rev)
    {
        int t=L[A.p1].last;
        A.Delete(t,t);
        A.Insert(t,t,A.Head);
    }
}

void Add(int vv)
{
    del+=vv;
}
void Reverse()
{
    Rev^=1;
}
void Insert(int vv)
{
    int t=L[A.Head].next;
    if(Rev)t=L[L[A.p1].last].last;
    A.Insert(vv-del,t);
    moveleft();
}
void Delete()
{
    moveright();
    int t=L[A.Head].next;
    if(Rev)t=L[A.p1].last;
    A.Delete(t,t);
}
void Move(int tp)
{
    int t;
    if(tp==1)
    {
        moveleft();
        t=L[A.End].last;
        A.Delete(t,t);
        if(!Rev)A.Insert(t,t,A.Head);
        else A.Insert(t,t,L[A.p1].last);
        L[t].v-=del;
    }
    else
    {
        t=L[A.Head].next;
        if(Rev)t=L[A.p1].last;
        A.Delete(t,t);
        A.Insert(t,t,L[A.End].last);
        L[t].v+=del;
        moveright();
    }
}
int Query()
{
    int t=L[A.Head].next;
    if(Rev)t=L[A.p1].last;
    return del+L[t].v;
}

int main()
{
    int i,j,n,m,k1,k2,ca,tp,a;
    for(ca=1;; ca++)
    {
        scanf("%d%d%d%d",&n,&m,&k1,&k2);
        if(n==0)break;
        tot=0;
        A.init();
        tp=A.Head;
        for(i=1; i<=n; i++)
        {
            scanf("%d",&a);
            A.Insert(a,tp);
            tp=L[tp].next;
        }
        A.p1=A.Head;
        for(i=0; i<=k1; i++)
            A.p1=L[A.p1].next;
        A.p2=A.Head;
        for(i=0; i<=k2; i++)
            A.p2=L[A.p2].next;
        del=0;Rev=0;
        printf("Case #%d:\n",ca);
        for(j=1; j<=m; j++)
        {
            char buff[20];
            int x;
            scanf("%s",buff);
            if(buff[0]=='q')printf("%d\n",Query());
            else if(buff[0]=='m')
            {
                scanf("%d",&x);
                Move(x);
            }
            else if(buff[0]=='a')
            {
                scanf("%d",&x);
                Add(x);
            }
            else if(buff[0]=='i')
            {
                scanf("%d",&x);
                Insert(x);
            }
            else if(buff[0]=='d')Delete();
            else if(buff[0]=='r')Reverse();
            else assert(0);
        }
    }
    return 0;
}
