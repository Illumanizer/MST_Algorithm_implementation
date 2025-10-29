// kruskal.cpp
// Kruskal's MST as a free function

#include "kruskal.h"

using namespace std;

// local helper: ensure edge stored as (min(u,v), max(u,v))
static void normalizeEdge(Edge &e) {
    if (e.u > e.v) {
        int t = e.u;
        e.u = e.v;
        e.v = t;
    }
}

// local helper: insertion sort edges by (w, u, v)
static void sortEdges(vector<Edge> &edges) {
    for (int i = 1; i < edges.size(); ++i) {
        Edge key = edges[i];
        int j = (int)i - 1;

        while (j >= 0) {
            bool greater = false;

            if (edges[j].w > key.w) greater = true;
            else if (edges[j].w == key.w && edges[j].u > key.u) greater = true;
            else if (edges[j].w == key.w && edges[j].u == key.u && edges[j].v > key.v) greater = true;

            if (!greater) break;
            edges[j + 1] = edges[j];
            --j;
        }
        edges[j + 1] = key;
    }
}

// public free function used by main.cpp
pair<vector<Edge>, long long> kruskal_mst(const Graph &G) {
    // copy edges so original graph is untouched
    vector<Edge> edges = G.edges;

    // normalize edges (u <= v)
    for (int i = 0; i < edges.size(); ++i)
        normalizeEdge(edges[i]);

    // sort by (weight, u, v)
    sortEdges(edges);

    // Kruskal main loop
    UnionFind uf(G.n);
    vector<Edge> mst;
    long long total = 0;

    for (int i = 0; i < edges.size(); ++i) {
        Edge &e = edges[i];
        if (!uf.same(e.u, e.v)) {
            uf.unite(e.u, e.v);
            mst.push_back(e);
            total += e.w;
            if ((int)mst.size() == G.n - 1) break;
        }
    }

    return {mst, total};
}
