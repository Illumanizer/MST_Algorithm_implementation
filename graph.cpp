// graph.cpp
// Lightweight Graph ADT for the MST task (undirected, weighted).
// Uses 1-based vertex indexing.

#include "graph.h"
using namespace std;
Graph::Graph(int nodes)
    : n(nodes),      // number of vertices
      adj(nodes + 1) // index 1..n
{
}

// add an undirected edge (a,b) with weight w
void Graph::add_edge(int a, int b, long long w)
{
    if (a < 1 || a > n || b < 1 || b > n)
        throw out_of_range("vertex index");

    adj[a].push_back({b, w});
    adj[b].push_back({a, w});

    // store once in edges list as (min, max)
    if (a < b)
        edges.emplace_back(a, b, w);
    else
        edges.emplace_back(b, a, w);
}

// build graph from an n x n cost matrix (use -1 for no edge)
Graph Graph::from_cost_matrix(const vector<vector<long long>> &mat)
{
    int n = (int)mat.size();
    Graph G(n);

    // read only upper triangle to avoid duplicates
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            long long w = mat[i][j];
            if (w >= 0)
                G.add_edge(i + 1, j + 1, w); // convert to 1-based
        }
    }
    return G;
}
