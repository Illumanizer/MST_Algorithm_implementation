#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <utility>
#include <stdexcept>
using namespace std; // make standard library names shorter

// structure for an edge (u,v) with weight w
struct Edge
{
    int u, v;
    int w;
    Edge(int a = 0, int b = 0, int c = 0) : u(a), v(b), w(c) {}
};

// graph class for adjacency list + edge list
class Graph
{
public:
    int n;
    vector<vector<pair<int, int>>> adj;
    vector<Edge> edges;

    Graph(int nodes = 0);
    void add_edge(int a, int b, int w);                            // add undirected edge
    static Graph from_cost_matrix(const vector<vector<int>> &mat); // build from cost matrix
};

#endif // GRAPH_H