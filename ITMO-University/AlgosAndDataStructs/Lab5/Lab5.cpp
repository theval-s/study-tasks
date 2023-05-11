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

int bor[1000][52];
//if char < 'a'   -65
//else - 71
int trie[500][26];
int val[500];
int idx = 0;
void insert(string s, int v) {
    int u = 0;
    for (char c : s) {
        int x = c - 'a';
        if (!trie[u][x]) {
            trie[u][x] = ++idx;
        }
        u = trie[u][x];
    }
    val[u] = v;
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

    insert("asdf", 1);
    //task2
    return 0;
}