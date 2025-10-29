#ifndef KRUSKAL_H
#define KRUSKAL_H

#include "graph.h"
#include "unionfind.h"
#include <vector>
#include <utility>

using namespace std;

// Kruskal's algorithm (free function).
// Returns pair(mstEdges, totalWeight).
pair<vector<Edge>, long long> kruskal_mst(const Graph &G);

#endif // KRUSKAL_H
