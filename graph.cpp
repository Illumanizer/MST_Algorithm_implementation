#include "graph.h"
using namespace std;

Graph::Graph(int nodes)
    : n(nodes), adj(nodes + 1) {}

void Graph::add_edge(int a, int b, int w)
{
    adj[a].push_back({b, w});
    adj[b].push_back({a, w});
    if (a < b)
        edges.push_back({a, b, w});
    else
        edges.push_back({b, a, w});
}

Graph Graph::from_cost_matrix(const vector<vector<int>> &mat)
{
    int size = mat.size();
    Graph G(size);
    for (int i = 0; i < size; ++i)
    {
        for (int j = i + 1; j < size; ++j)
        {
            int w = mat[i][j];
            if (w >= 0)
                G.add_edge(i + 1, j + 1, w);
        }
    }
    return G;
}