#include "prim.h"
#include "fibheap.h"
#include <climits>
using namespace std;

// compute MST using Prim and Fibonacci-like heap (C-style)
pair<vector<Edge>, int> prim_mst(const Graph &G)
{
    int n = G.n;
    if (n <= 0)
        return {{}, 0};

    vector<int> key(n + 1, INT_MAX); // best edge cost to connect vertex
    vector<int> parent(n + 1, -1);   // parent in MST
    vector<int> inMST(n + 1, 0);     // visited flag

    // create heap and node mapping
    fibheap_c *H = fh_create();               // create heap
    vector<fh_node *> nodeOf(n + 1, nullptr); // map vertex -> node pointer

    // insert all vertices with INF key
    for (int v = 1; v <= n; ++v)
        nodeOf[v] = fh_insert(H, v, INT_MAX);
    

    // start from vertex 1
    key[1] = 0;
    if (nodeOf[1])
        fh_decrease_key(H, nodeOf[1], 0);

    vector<Edge> mst;
    int total = 0;

    // main loop
    while (!fh_empty(H))
    {
        fh_node *minnode = fh_extract_min_node(H); // remove min
        if (!minnode)
            break;
        int u = minnode->vertex;
        // free extracted node memory
        delete minnode;
        nodeOf[u] = nullptr; // mark removed

        if (key[u] == INT_MAX)
            break; // remaining nodes unreachable
        inMST[u] = 1;
        if (parent[u] != -1)
        {
            mst.push_back({parent[u], u, key[u]});
            total += key[u];
        }

        // relax neighbors
        for (int i = 0; i < G.adj[u].size(); ++i)
        {
            int v = G.adj[u][i].first;
            int w = G.adj[u][i].second;
            if (inMST[v])
                continue;
            if (w < key[v])
            {
                key[v] = w;
                parent[v] = u;
                if (nodeOf[v])
                    fh_decrease_key(H, nodeOf[v], w);
                
            }
        }
    }

    // cleanup heap and remaining nodes
    fh_destroy(H);

    return {mst, total};
}


// n insert * O(1) - O(n)
// m decrease_key * O(1) - O(m)
// n extract_min * O(log n) - O(n log n)

// overall amortized: O(m + n log n)

