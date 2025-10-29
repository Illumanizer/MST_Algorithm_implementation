#ifndef PRIM_H
#define PRIM_H

#include "graph.h"
#include "fibheap.h"
#include <vector>
#include <utility>


using namespace std;

// returns MST edges and total cost using Prim's algorithm
pair<vector<Edge>, long long> prim_mst(const Graph &G);

#endif
