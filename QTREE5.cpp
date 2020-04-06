#include<bits/stdc++.h>
using namespace std;
const int N=1e5+5;
int n,q,lca_parent[N][25],nd,level[N],cnt[N],centroid_par[N];
vector<int>vertex[N];
//vector<pair<int,int> >;
bool visit_centroid[N],node_ck[N];
multiset< pair<int,int>  >ans[N],vt[N];
void dfs(int node,int p) {
    for(int i=1; i<=17; i++) {
        lca_parent[node][i]=lca_parent[lca_parent[node][i-1]][i-1];
    }
    for(int v:vertex[node]) {
        if(v!=p) {
            level[v]=level[node]+1;
            lca_parent[v][0]=node;
            dfs(v,node);
        }
    }
}

int lca_par(int node1,int node2) {
    if(level[node1]<level[node2])swap(node1,node2);
    for(int i=17; i>=0; i--) {
        if(level[node1]-(1<<i)>=level[node2]) {
            node1=lca_parent[node1][i];
        }
    }
    if(node1==node2)return node1;

    for(int i=17; i>=0; i--) {
        if(lca_parent[node1][i]!=lca_parent[node2][i]) {
            node1=lca_parent[node1][i],node2=lca_parent[node2][i];
        }
    }
    return lca_parent[node1][0];
}

int dis(int node1,int node2,int p) {
    return level[node1]+level[node2]-2*level[p];
}

/****************************************************************/

void getsz(int node,int p) {
    nd++;
    cnt[node]=1;
    for(int v:vertex[node]) {
        if(p!=v&&!visit_centroid[v]) {
            getsz(v,node);
            cnt[node]+=cnt[v];
        }
    }
}

int centroid(int node,int p) {
    for(int v:vertex[node]) {
        if(p!=v&&!visit_centroid[v]&&cnt[v]>nd/2) {
            return centroid(v,node);
        }
    }
    return node;
}

void decompose(int node,int p) {
    nd=0;
    getsz(node,0);
    int center=centroid(node,p);
    if(p==0)p=center;
    centroid_par[center]=p;
    visit_centroid[center]=true;
    for(int v:vertex[center]) {
        if(!visit_centroid[v]&&v!=p) {
            decompose(v,center);
        }
    }
}

void update(int node) {
    if(node_ck[node])node_ck[node]=false;
    else node_ck[node]=true;
    int x=node;
    if(node_ck[node]) {
        while(1) {
            int d=dis(x,node,lca_par(x,node));
            vt[x].insert({node,d});
            ans[x].insert({d,node});
            if(x==centroid_par[x])break;
            x=centroid_par[x];

        }
    } else {
        while(1) {
            auto it1=vt[x].lower_bound(make_pair(node,INT_MIN));
            pair<int,int>val=*it1;
            auto it=ans[x].lower_bound({val.second,node});
            //if(it==ans[x].end()){
            //cout<<val.first<<"** "<<node<<endl;
            //}
            ans[x].erase(it);
            vt[x].erase(it1);
            if(x==centroid_par[x])break;
            x=centroid_par[x];

        }
    }
}

int query(int node) {
    int x=node,res=1e6;
    pair<int,int>value;
    while(1) {
        if(ans[x].size()==0) {
            value.first=1e6;
        } else {
            auto it=ans[x].begin();
            value=*it;
        }
        res=min(res,value.first+dis(x,node,lca_par(x,node)));
        if(x==centroid_par[x])break;
        x=centroid_par[x];
    }
    return res;
}
int main() {
    int x,y;
    scanf("%d",&n);
    for(int i=1; i<n; i++) {
        scanf("%d %d",&x,&y);
        vertex[x].push_back(y);
        vertex[y].push_back(x);
    }
    dfs(1,0);
    decompose(1,0);
    scanf("%d",&q);
    while(q--) {
        int ck;
        scanf("%d %d",&ck,&x);
        if(ck==0) {
            update(x);
        } else {
            int res=query(x);
            if(res>=1e6) {
                printf("-1\n");
            } else
                printf("%d\n",res);
        }
    }
    return 0;
}
