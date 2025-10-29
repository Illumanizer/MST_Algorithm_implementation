
// ===== File: prim.h =====
#ifndef PRIM_H
#define PRIM_H

#include "graph.h"
#include <vector>
#include <utility>
using namespace std;

// function to compute MST using Prim’s algorithm
pair<vector<Edge>, int> prim_mst(const Graph &G);

#endif // PRIM_H