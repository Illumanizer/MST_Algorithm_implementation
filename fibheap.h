#ifndef FIBHEAP_H
#define FIBHEAP_H

#include <climits>

struct fh_node
{
    int vertex;                             // vertex id stored in this node
    int key;                                // priority key
    int degree;                             // number of children
    bool mark;                              // marked flag for cascading cuts
    fh_node *left, *right, *parent, *child; // pointers for circular lists
    fh_node(int v = -1, int k = INT_MAX)
        : vertex(v), key(k), degree(0), mark(false), left(this), right(this), parent(nullptr), child(nullptr) {}
};

struct fibheap_c
{
    int nodes;       // number of nodes in heap
    fh_node *minptr; // pointer to min node
    fibheap_c() : nodes(0), minptr(nullptr) {}
};

// create and destroy heap
fibheap_c *fh_create();
void fh_destroy(fibheap_c *H);

// insert vertex with key and return node pointer
fh_node *fh_insert(fibheap_c *H, int vertex, int key);

// extract min node (caller owns returned pointer and should delete it)
fh_node *fh_extract_min_node(fibheap_c *H);

// decrease key of a given node
void fh_decrease_key(fibheap_c *H, fh_node *x, int newKey);

// check empty
bool fh_empty(const fibheap_c *H);

#endif // FIBHEAP_H