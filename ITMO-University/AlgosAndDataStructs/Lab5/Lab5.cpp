#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
const int INF = 1e9;

//task 1 - Prim's Algorithm

vector<vector<pair<int, int>>> adj;
vector<int> dist;
/// <summary>
/// builds the spanning tree of a graph using Prim's algorithm
/// </summary>
/// <param name="start">starting node</param>
/// <param name="n">amount of nodes</param>
void prim(int start, int n) {
    vector<int> included_nodes = { start };
    vector<bool> isIncluded(n + 1, 0);
    dist[start] = 0;
    isIncluded[start] = true;
    while (included_nodes.size() < n) {
        int min = INF, node;
        for (int k : included_nodes) {
            for (auto edge : adj[k]) if (!isIncluded[edge.first] && edge.second < min) {
                min = edge.second;
                node = edge.first;
            }
        }
        dist[node] = min;
        isIncluded[node] = true;
        included_nodes.push_back(node);
    }
}




//task 2 - Aho-Corasicks Algo

//if char < 'a'   -65
//else - 71
int trie[1000][52];
int val[1000];
int fail[1000];
int idx = 0;
void insert(string s, int v) {
    int u = 0;
    for (char c : s) {
        int x = ((int)c < (int)'a')?(c-'A'):((int)c -71);
        if (!trie[u][x]) {
            trie[u][x] = ++idx;
        }   
        u = trie[u][x];
    }
    val[u] = v;
}
void build() {
    queue<int> q;
    for (int i = 0; i < 52; i++) {
        if (trie[0][i]) {
            q.push(trie[0][i]);
        }
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int i = 0; i < 52; i++) {
            int v = trie[u][i];
            if (!v) {
                trie[u][i] = trie[fail[u]][i];
                continue;
            }
            fail[v] = trie[fail[u]][i];
            q.push(v);
        }
    }
}
vector<int> find(string s, int n) {
    vector<int> ans;
    int u = 0;
    for (int i = 0; i < n; i++) {
        int x = ((int)s[i] < (int)'a') ? (s[i] - 'A') : ((int)s[i] - 71);
        u = trie[u][x];
        int v = u;
        while (v > 0) {
            if (val[v]) {
                ans.push_back(i-idx+2);
            }
            v = fail[v];
        }
    }
    return ans;
}

int main() {
    //task1
    /*
    int n, m;
    cin >> n >> m;
    adj.resize(n + 1, vector<pair<int,int>>());
    dist.resize(n + 1, INF);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back(make_pair(v, w));
        adj[v].push_back(make_pair(u, w));
    }
    prim(1, m);
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        ans += dist[i];
    }
    cout << ans << '\n';
    */

    //task2
    string p, t;
    cin >> p >> t;
    insert(p, 1);
    build();
    vector<int> ans = find(t, t.size());
    cout << ans.size() << endl;
    for (auto k : ans) {
        cout << k << endl;
    }
    return 0;
}