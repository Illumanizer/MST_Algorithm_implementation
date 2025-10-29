#include "unionfind.h"
using namespace std;

// constructor initializes parent and rank arrays
UnionFind::UnionFind(int n)
    : parent(n + 1), rankv(n + 1, 0)
{
    for (int i = 0; i <= n; ++i)
        parent[i] = i; // initially, each element is its own parent (self root)
}

// find the root (representative) of a set with path compression
int UnionFind::find(int x)
{
    if (parent[x] != x) // if x is not its own parent, recursively find and compress
        parent[x] = find(parent[x]);
    return parent[x];
}

// merge two sets (union by rank)
void UnionFind::unite(int a, int b)
{
    int ra = find(a);
    int rb = find(b);
    // attach smaller tree under larger one
    if (ra == rb)
        return;
    if (rankv[ra] < rankv[rb])
        parent[ra] = rb;
    else if (rankv[ra] > rankv[rb])
        parent[rb] = ra;
    else
    {
        // if ranks equal, choose one as root and increment its rank
        parent[rb] = ra;
        rankv[ra]++;
    }
}
// check if two elements belong to the same set
bool UnionFind::same(int a, int b)
{
    return find(a) == find(b);
}