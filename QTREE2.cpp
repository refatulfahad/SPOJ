#include<bits/stdc++.h>
using namespace std;
#define ll long long
const int N=1e4+5;
int n,ar[N],level[N],sz[N],par[N][18],chain_id[N],ar_index[N],num;
vector<int>vertex[N],chain[N],cost[N],bit[N];


void dfs(int node,int p) {
    sz[node]=1;
    for(int i=1; i<=14; i++) {
        par[node][i]=par[par[node][i-1]][i-1];
    }
    for(int i=0; i<(int)vertex[node].size(); i++) {
        int v=vertex[node][i];
        if(v!=p) {
            par[v][0]=node;
            ar[v]=cost[node][i];
            level[v]=level[node]+1;
            dfs(v,node);
            sz[node]+=sz[v];
        }
    }
}

int lca_par(int node1,int node2) {
    if(level[node1]<level[node2])swap(node1,node2);
    for(int i=14; i>=0; i--) {
        if(level[node1]-(1<<i)>=level[node2]) {
            node1=par[node1][i];
        }
    }
    if(node1==node2)return node1;

    for(int i=14; i>=0; i--) {
        if(par[node1][i]!=par[node2][i]) {
            node1=par[node1][i],node2=par[node2][i];
        }
    }
    return par[node1][0];
}

int dis(int node1,int node2) {
    return level[node1]-level[node2];
}
/**************************************************************/

void update(int len,int id,int pos,int val) {
    while(pos<=len) {
        bit[id][pos]+=val;
        pos+=(pos&-pos);
    }
}

int query(int id,int pos) {
    int ans=0;
    while(pos>0) {
        ans+=bit[id][pos];
        pos-=(pos&-pos);
    }
    return ans;
}

void initial() {
    for(int i=1; i<=num; i++) {
        for(int j=0; j<=(int)chain[i].size(); j++) {
            bit[i].push_back(0);
        }
        for(int j=1; j<=(int)chain[i].size(); j++) {
            update(chain[i].size(),i,j,ar[chain[i][j-1]]);
        }
    }
}

void hld(int node,int p) {
    chain[num].push_back(node),chain_id[node]=num;
    ar_index[node]=chain[num].size();
    int special_child=-1,mx=-1;
    for(int v:vertex[node]) {
        if(v!=p&&sz[v]>mx) {
            mx=sz[v];
            special_child=v;
        }
    }

    if(special_child!=-1) {
        hld(special_child,node);
    }
    for(int v:vertex[node]) {
        if(v!=p&&special_child!=v) {
            num++;
            hld(v,node);
        }
    }
}

int hld_query(int node1,int node2) {
    int lca=lca_par(node1,node2);
    int ans=0;
    //cout<<"lca"<<lca<<endl;
    while(1) {
        if(node1==lca)break;
        if(chain_id[node1]==chain_id[lca]) {
            ans+=(query(chain_id[node1],ar_index[node1])-query(chain_id[lca],ar_index[lca]));
            //cout<<ans<<" ";
            break;
        }
        ans+=query(chain_id[node1],ar_index[node1]);
        //cout<<query(chain_id[node1],ar_index[node1])<<" ";
        node1=par[chain[chain_id[node1]][0]][0];
    }
    //cout<<ans<<" ";
    while(1) {
        if(node2==lca)break;
        if(chain_id[node2]==chain_id[lca]) {
            ans+=(query(chain_id[node2],ar_index[node2])-query(chain_id[lca],ar_index[lca]));
            break;
        }
        ans+=query(chain_id[node2],ar_index[node2]);
        //cout<<ans<<" ";
        node2=par[chain[chain_id[node2]][0]][0];
        //cout<<node2<<" ";
    }
    return ans;
}

int main() {
    int test,x,y,z,ans;
    int pre;

    scanf("%d",&test);
    for(int cs=1; cs<=test; cs++) {
        scanf("%d",&n);
        for(int i=1; i<n; i++) {
            scanf("%d %d %d",&x,&y,&z);
            vertex[x].push_back(y);
            cost[x].push_back(z);
            vertex[y].push_back(x);
            cost[y].push_back(z);
        }
        //cout<<ar[1]<<endl;
        num=1;
        level[1]=0;
        par[1][0]=1;
        dfs(1,0);
        hld(1,0);
        initial();
        //cout<<num<<endl;
        while(1) {
            string s;
            int from,to;
            cin>>s;
            if(s=="DIST") {
                scanf("%d %d",&from,&to);
                printf("%d\n",hld_query(from,to));
            } else if(s=="KTH") {
                int k;
                scanf("%d %d %d",&from,&to,&k);
                int lca=lca_par(from,to);
                int dis1=dis(from,lca)+1;
                if(k<=dis1) {
                    //cout<<dis1<<" ";
                    dis1=k;
                    dis1-=1;
                    if(dis1==0)ans=from;
                    else {
                        ans=from;
                        int sum=dis1;
                        while(sum>0) {
                            pre=1;
                            for(int i=1; i<=log(sum); i++) {
                                pre*=2;
                            }
                            int in=log(sum);
                            ans=par[ans][in];
                            sum-=pre;
                        }
                    }
                } else {
                    int dis2=dis(to,lca);
                    dis2-=(k-dis1);
                    if(dis2==0)ans=to;
                    else {
                        ans=to;
                        int sum=dis2;
                        while(sum>0) {
                            pre=1;
                            for(int i=1; i<=log(sum); i++) {
                                pre*=2;
                            }
                            int in=log(sum);
                            ans=par[ans][in];
                            sum-=pre;
                        }
                    }

                }
                printf("%d\n",ans);
            } else {
                break;
            }
        }
        for(int i=0; i<=n; i++) {
            ar[i]=0,level[i]=0;
            vertex[i].clear(),chain[i].clear(),bit[i].clear(),cost[i].clear(),ar_index[i]=0;
            for(int j=0; j<=14; j++)par[i][j]=0;
        }
        printf("\n");
    }
    return 0;
}
