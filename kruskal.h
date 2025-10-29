#ifndef KRUSKAL_H
#define KRUSKAL_H

#include "graph.h"
#include "unionfind.h"
#include <vector>
#include <utility>
using namespace std;

// function to compute MST using Kruskal algorithm
pair<vector<Edge>, int> kruskal_mst(const Graph &G);

#endif // KRUSKAL_H
