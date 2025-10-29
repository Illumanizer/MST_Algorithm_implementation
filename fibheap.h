#ifndef FIBHEAP_H
#define FIBHEAP_H

#include <vector>
#include <limits>
#include <functional>

using ll = long long;

using namespace std;

struct FibNode
{
    int vertex; // associated vertex id
    ll key;     // priority/key
    int degree;
    bool mark;
    FibNode *left, *right, *parent, *child;
    FibNode(int v = -1, ll k = numeric_limits<ll>::max())
        : vertex(v), key(k), degree(0), mark(false),
          left(this), right(this), parent(nullptr), child(nullptr) {}
};

class FibHeap
{
public:
    explicit FibHeap(int maxVertices = 0); // optionally give number of vertices
    ~FibHeap();                            // destructor

    void insert(int vertex, ll key);
    bool contains(int vertex) const;
    void decrease_key(int vertex, ll newKey);
    int extract_min();  // returns vertex id (or -1 if empty)
    ll min_key() const; // returns key of min (LLONG_MAX if empty)
    bool empty() const;
    void clear(); // destroy all nodes

private:
    int nodesCount;
    FibNode *minptr;
    vector<FibNode *> nodesById; // index by vertex id (size = maxVertices+1), null if absent

    // internal helpers
    static void removeFromList(FibNode *x);
    static void link(FibNode *parentNode, FibNode *childNode);
    void consolidate();
    void moveChildrenToRootList(FibNode *z);
    void freeAllNodes();
};

#endif // FIBHEAP_H
