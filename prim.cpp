// prim.cpp
// Prim's algorithm implementation using the FibHeap priority queue.
// Returns a pair: (vector of MST edges, total weight)

#include "prim.h"
#include "fibheap.h" // FibHeap class: insert(v,key), decrease_key(v,key), extract_min(), contains(v), empty()
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

// prim_mst:
// - Input: Graph G (1-based vertices, G.n vertices)
// - Output: pair(mstEdges, totalWeight)
pair<vector<Edge>, long long> prim_mst(const Graph &G)
{
    const long long INF = numeric_limits<long long>::max();
    int n = G.n;

    // If graph has zero vertices, return empty result
    if (n <= 0)
        return {{}, 0LL};

    // Prepare adjacency lists sorted deterministically (by weight, then vertex)
    vector<vector<pair<int, long long>>> adj_sorted(n + 1);
    for (int u = 1; u <= n; ++u)
    {
        adj_sorted[u] = G.adj[u]; // copy
        sort(adj_sorted[u].begin(), adj_sorted[u].end(),
                  [](const pair<int, long long> &a, const pair<int, long long> &b)
                  {
                      if (a.second != b.second)
                          return a.second < b.second;
                      return a.first < b.first;
                  });
    }

    // Data structures for Prim
    vector<long long> key(n + 1, INF); // best known edge weight to connect vertex
    vector<int> parent(n + 1, -1);     // parent in MST
    vector<char> inMST(n + 1, 0);      // whether vertex is already added to MST

    // Priority queue using FibHeap; give maximum vertex id so it can index nodesById
    FibHeap pq(n);

    // Initialize: set all vertices with infinite key, then set start vertex (1) to 0
    for (int v = 1; v <= n; ++v)
    {
        key[v] = INF;
        parent[v] = -1;
        pq.insert(v, key[v]);
    }

    // Start Prim from vertex 1
    key[1] = 0;
    if (pq.contains(1))
    {
        pq.decrease_key(1, 0);
    }
    else
    {
        pq.insert(1, 0);
    }

    vector<Edge> mstEdges;
    long long totalWeight = 0;

    // Main loop
    while (!pq.empty())
    {
        int u = pq.extract_min(); // returns vertex id or -1 if empty
        if (u == -1)
            break;

        // If the smallest key is INF, remaining vertices are unreachable
        if (key[u] == INF)
            break;

        inMST[u] = 1;
        // If u has a parent, add the edge to MST
        if (parent[u] != -1)
        {
            mstEdges.emplace_back(parent[u], u, key[u]);
            totalWeight += key[u];
        }

        // Relax edges from u
        for (const auto &pr : adj_sorted[u])
        {
            int v = pr.first;
            long long w = pr.second;
            if (inMST[v])
                continue;
            if (w < key[v])
            {
                key[v] = w;
                parent[v] = u;
                if (pq.contains(v))
                    pq.decrease_key(v, w);
                else
                    pq.insert(v, w); // safety: if not present, insert it
            }
        }
    }

    return {mstEdges, totalWeight};
}
