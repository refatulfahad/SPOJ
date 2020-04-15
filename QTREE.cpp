#include<cstdio>
#include<vector>
#include<string>
using namespace std;

const int N=1e4+5;
vector<int>vertex[N],index[N],cost[N];
int par[N][18],sz[N],level[N],n,ar[4*N],chain_id[N],ar_index[N],chain_head[N],incr,ar_cost[N],snd_node[N],num;


void dfs(int node,int p) {
    sz[node]=1;
    for(int i=1; i<=15; i++) {
        par[node][i]=par[par[node][i-1]][i-1];
    }
    for(int i=0; i<vertex[node].size(); i++) {
        int v=vertex[node][i];
        if(v!=p) {
            level[v]=level[node]+1;
            par[v][0]=node;
            snd_node[index[node][i]]=v;
            dfs(v,node);
            sz[node]+=sz[v];
        }
    }
}

int lca_par(int node1,int node2) {
    if(level[node1]<level[node2])swap(node1,node2);
    for(int i=15; i>=0; i--) {
        if(level[node1]-(1<<i)>=level[node2]) {
            node1=par[node1][i];
        }
    }
    if(node1==node2)return node1;

    for(int i=15; i>=0; i--) {
        if(par[node1][i]!=par[node2][i]) {
            node1=par[node1][i],node2=par[node2][i];
        }
    }
    return par[node1][0];
}

/*********************************************************/

void update(int node_num,int l,int r,int pos,int val) {
    if(l>pos||r<pos)return;
    if(l==r) {
        ar[node_num]=val;
        return;
    }
    int mid=(l+r)>>1;
    int left=node_num<<1;
    int right=left|1;
    update(left,l,mid,pos,val);
    update(right,mid+1,r,pos,val);
    ar[node_num]=max(ar[left],ar[right]);
}

int query(int node_num,int l,int r,int from,int to) {
    if(r<from||l>to)return -1;
    if(l>=from&&r<=to) {
        //cout<<ar[node_num];
        return ar[node_num];
    }
    int mid=(l+r)>>1;
    int left=node_num<<1;
    int right=left|1;
    int x=query(left,l,mid,from,to);
    int y=query(right,mid+1,r,from,to);
    if(x>y)return x;
    else return y;
}

void hld(int node,int costs,int p) {
    if(chain_head[num]==0) {
        chain_head[num]=node;
    }
    chain_id[node]=num;
    incr++;
    ar_index[node]=incr;
    ar_cost[incr]=costs;
    int special_child=-1,mx=-1,tem_cost;
    for(int i=0; i<vertex[node].size(); i++) {
        int v=vertex[node][i];
        if(v!=p&&sz[v]>mx) {
            mx=sz[v];
            special_child=v;
            tem_cost=cost[node][i];
        }
    }

    if(special_child!=-1) {
        //cout<<tem_cost<<" ";
        hld(special_child,tem_cost,node);
    }
    for(int i=0; i<vertex[node].size(); i++) {
        int v=vertex[node][i];
        if(v!=p&&special_child!=v) {
            num++;
            hld(v,cost[node][i],node);
        }
    }
}

void initial(int node_num,int l,int r) {
    if(l==r) {
        ar[node_num]=ar_cost[l];
        return;
    }
    int left=node_num<<1;
    int right=left|1;
    int mid=(l+r)>>1;
    initial(left,l,mid);
    initial(right,mid+1,r);
    ar[node_num]=max(ar[left],ar[right]);
}

int hld_query(int node1,int node2) {
    int lca=lca_par(node1,node2),ans=0;
    while(1) {
        int tem=chain_id[node1];
        if(node1==lca)ans=max(0,ans);
        if(chain_id[node1]==chain_id[lca]) {
            //cout<<ar_index[lca]+1<<ar_index[node1];
            ans=max(ans,query(1,1,incr,ar_index[lca]+1,ar_index[node1]));
            break;
        }
        ans=max(ans,query(1,1,incr,ar_index[chain_head[chain_id[node1]]],ar_index[node1]));
        node1=par[chain_head[chain_id[node1]]][0];
    }
    //cout<<ans<<endl;
    while(1) {
        int tem=chain_id[node2];
        if(node2==lca)ans=max(0,ans);
        if(chain_id[node2]==chain_id[lca]) {
            //cout<<ar_index[lca]+1<<" "<<ar_index[node2]+1;
            ans=max(ans,query(1,1,incr,ar_index[lca]+1,ar_index[node2]));
            break;
        }
        ans=max(ans,query(1,1,incr,ar_index[chain_head[chain_id[node2]]],ar_index[node2]));
        node2=par[chain_head[chain_id[node2]]][0];
    }
    return ans;
}
int main() {
    int test,x,y,z;
    scanf("%d",&test);
    for(int cs=1; cs<=test; cs++) {
        scanf("%d",&n);
        for(int i=1; i<n; i++) {
            scanf("%d %d %d",&x,&y,&z);
            vertex[x].push_back(y);
            index[x].push_back(i);
            cost[x].push_back(z);
            vertex[y].push_back(x);
            index[y].push_back(i);
            cost[y].push_back(z);
        }
        incr=0;
        level[1]=0;
        par[1][0]=1;
        dfs(1,0);
        num=1;
        hld(1,-1,0);
        initial(1,1,incr);
        //cout<<ar[4]<<" "<<ar[5]<<" "<<ar[3]<<" "<<incr;
        while(1) {
            char s[15];
            int l,r,pos,val;
            scanf("%s",s);
            if(s[0]=='D') {
                break;
            } else if(s[0]=='Q') {
                scanf("%d %d",&l,&r);
                printf("%d\n",hld_query(l,r));
            } else {
                scanf("%d %d",&pos,&val);
                //cout<<snd_node[pos];
                update(1,1,incr,ar_index[snd_node[pos]],val);
            }
        }
        for(int i=1; i<=n; i++) {
            vertex[i].clear(),index[i].clear(),cost[i].clear(),chain_head[i]=0;
            for(int j=0; j<=15; j++) {
                par[i][j]=0;
            }
        }
    }
    return 0;
}


