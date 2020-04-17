#include<bits/stdc++.h>
using namespace std;
const int N=1e5+5;
int n,level[N],sz[N],par[N][20],ar_index[N],chain_head[N],chain_id[N],ar_dis[N],num,incr,tree[4*N];
bool color[N];
vector<int>vertex[N];

void dfs(int node,int p) {
    sz[node]=1;
    for(int i=1; i<=17; i++) {
        par[node][i]=par[par[node][i-1]][i-1];
    }
    for(int v:vertex[node]) {
        if(v!=p) {
            level[v]=level[node]+1;
            par[v][0]=node;
            dfs(v,node);
            sz[node]+=sz[v];
        }
    }
}

int lca_par(int node1,int node2) {

    for(int i=17; i>=0; i--) {
        if((1<<i)<=node2) {
            node1=par[node1][i];
            node2-=(1<<i);
        }
    }

    return node1;
}

void hld(int node,int p,int value) {
    if(chain_head[num]==0) {
        chain_head[num]=node;
    }
    chain_id[node]=num;
    ar_index[node]=incr;
    ar_dis[incr]=value;
    incr++;
    int special_child=-1,mx=-1;
    for(int v:vertex[node]) {
        if(v!=p&&mx<sz[v]) {
            mx=sz[v];
            special_child=v;
        }
    }
    if(special_child!=-1) {
        hld(special_child,node,color[special_child]?INT_MAX:level[special_child]);
    }
    for(int v:vertex[node]) {
        if(v!=p&&special_child!=v) {
            num++;
            hld(v,node,color[special_child]?INT_MAX:level[special_child]);
        }
    }
}

void initial(int node,int l,int r) {
    if(l==r) {
        tree[node]=ar_dis[l];
        return;
    }
    int mid=(l+r)>>1;
    int left=node<<1;
    int right=left|1;
    initial(left,l,mid);
    initial(right,mid+1,r);
    tree[node]=min(tree[left],tree[right]);
}

void update(int node,int l,int r,int pos,int value) {
    if(l>pos||r<pos)return;
    if(l==pos&&r==pos) {
        tree[node]=value;
        return;
    }
    int mid=(l+r)>>1;
    int left=node<<1;
    int right=left|1;
    update(left,l,mid,pos,value);
    update(right,mid+1,r,pos,value);
    tree[node]=min(tree[left],tree[right]);
}

int query(int node,int l,int r,int from,int to) {
    if(l>to||r<from)return INT_MAX;
    if(l>=from&&r<=to) {
        return tree[node];
    }
    int mid=(l+r)>>1;
    int left=node<<1;
    int right=left|1;
    int l1=query(left,l,mid,from,to);
    int r1=query(right,mid+1,r,from,to);

    return min(l1,r1);
}

int hld_query(int node2) {
    int ans=INT_MAX,node1;
    while(1) {
        if(chain_id[node2]==chain_id[1]) {
            ans=min(ans,query(1,1,n,ar_index[1],ar_index[node2]));
            break;
        }
        ans=min(ans,query(1,1,n,ar_index[chain_head[chain_id[node2]]],ar_index[node2]));
        node1=chain_head[chain_id[node2]];
        node2=par[node1][0];
    }
    return ans;
}
int main() {
    int q,test,x,y;
    for(int cs=1; cs<=1; cs++) {
        scanf("%d %d",&n,&q);
        for(int i=1; i<n; i++) {
            scanf("%d %d",&x,&y);
            vertex[x].push_back(y);
            vertex[y].push_back(x);
        }
        for(int i=1; i<=n; i++) {
            color[i]=true;
        }
        par[1][0]=1;
        incr=1;
        num=1;
        dfs(1,0);
        hld(1,0,INT_MAX);
        initial(1,1,n);
        while(q--) {
            int ck,node,value;
            scanf("%d %d",&ck,&node);
            if(ck) {
                int ck1=hld_query(node);
                if(ck1==INT_MAX) {
                    printf("-1\n");
                } else {
                    printf("%d\n",lca_par(node,level[node]-ck1));
                }
            } else {

                if(color[node])color[node]=false;
                else color[node]=true;
                if(color[node])value=INT_MAX;
                else value=level[node];
                update(1,1,n,ar_index[node],value);
            }
        }
    }
    return 0;
}
