#include "unionfind.h"
using namespace std;

UnionFind::UnionFind(int n)
    : parent(n + 1), rankv(n + 1, 0)
{
    for (int i = 0; i <= n; ++i)
        parent[i] = i;
}

int UnionFind::find(int x)
{
    if (parent[x] != x)
        parent[x] = find(parent[x]);
    return parent[x];
}

void UnionFind::unite(int a, int b)
{
    int ra = find(a);
    int rb = find(b);
    if (ra == rb)
        return;
    if (rankv[ra] < rankv[rb])
        parent[ra] = rb;
    else if (rankv[ra] > rankv[rb])
        parent[rb] = ra;
    else
    {
        parent[rb] = ra;
        rankv[ra]++;
    }
}

bool UnionFind::same(int a, int b)
{
    return find(a) == find(b);
}