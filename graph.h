#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <utility>
#include <stdexcept>

using namespace std;

// simple structure to store an undirected edge
struct Edge
{
    int u, v;
    long long w;
    Edge(int a = 0, int b = 0, long long c = 0) : u(a), v(b), w(c) {}
};

// basic weighted undirected graph
class Graph
{
public:
    int n;                                    // number of vertices
    vector<vector<pair<int, long long>>> adj; // adjacency list
    vector<Edge> edges;                       // list of all edges (u < v)

    Graph(int nodes = 0);
    void add_edge(int a, int b, long long w);
    static Graph from_cost_matrix(const vector<vector<long long>> &mat);
};

#endif
