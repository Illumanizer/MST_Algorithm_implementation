#ifndef UNIONFIND_H
#define UNIONFIND_H

#include <vector>
using namespace std;

// disjoint set (union-find) class
class UnionFind
{
private:
    vector<int> parent;
    vector<int> rankv;

public:
    UnionFind(int n);
    int find(int x);          // find root
    void unite(int a, int b); // merge sets
    bool same(int a, int b);  // check if in same set
};

#endif // UNIONFIND_H
