#include "kruskal.h"
using namespace std;

// ensure u <= v for consistency
static void normalize_edge(Edge &e)
{
    
    if (e.u > e.v) {
        int t = e.u;
        e.u = e.v;
        e.v = t;
    }
}

// insertion sort based on (w,u,v)
static void sort_edges(vector<Edge> &arr)
{
    for (int i = 1; i < arr.size(); ++i)
    {
        Edge key = arr[i];
        int j = i - 1;
        while (j >= 0)
        {
            bool greater = false;
            if (arr[j].w > key.w)
                greater = true;
            else if (arr[j].w == key.w && arr[j].u > key.u)
                greater = true;
            else if (arr[j].w == key.w && arr[j].u == key.u && arr[j].v > key.v)
                greater = true;
            if (!greater)
                break;
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

pair<vector<Edge>, int> kruskal_mst(const Graph &G)
{
    vector<Edge> sorted = G.edges;
    for (int i = 0; i < sorted.size(); ++i)
        normalize_edge(sorted[i]);
    sort_edges(sorted);

    UnionFind uf(G.n);
    vector<Edge> mst;
    int total = 0;

    for (int i = 0; i < sorted.size(); ++i)
    {
        Edge &e = sorted[i];
        if (!uf.same(e.u, e.v))
        {
            uf.unite(e.u, e.v);
            mst.push_back(e);
            total += e.w;
            if ((int)mst.size() == G.n - 1)
                break;
        }
    }
    return {mst, total};
}
