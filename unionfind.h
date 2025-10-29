#ifndef UNIONFIND_H
#define UNIONFIND_H

#include <vector>

using namespace std;

// simple union-find (disjoint set) with path compression and union by rank
class UnionFind {
private:
    vector<int> parent, rankv;

public:
    UnionFind(int n);
    int find(int x);
    void unite(int a, int b);
    bool same(int a, int b);
};

#endif
